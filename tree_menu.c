/*
 * tree_menu.c
 *
 *  Created on: 22 июн. 2020 г.
 *      Author: Maksim Cherkasov
 */

#include "tree_menu.h"

TM_Config* menuConfig;
bool init = false;

int actualIndex = 0;
TM_DeepLevel deep_level = DL_NAME; // уровень погружени€ 0 - им€ параметра, 1 - дельта, 2 - параметр
TM_ParamType tmpValue = 0; // ¬ременна€ переменна€ дл€ хранени€ измен€емого параметра

int findNeighbor(int parentid, int id, TM_Direction side);

void InitMenu(TM_Config* Configuration) {
	if (Configuration) {
		menuConfig = Configuration;
		init = true;
	}
}

int JumpMenu(TM_Direction Direction) {
	if (!init)
		return -1;
	switch (Direction) {
	case MD_UP: // двежемс€ вверх
		switch (deep_level) { // провер€ем уровень глубины
		case DL_NAME: //переход на родител€
			if (menuConfig->Menus[actualIndex].parent_id >= 0) { // ≈сли есть куда перемещатьс€ вверх (ParentID>0)
				actualIndex = menuConfig->Menus[actualIndex].parent_id;
			}
			break;
		case DL_DELTA: //отображаем им€
			deep_level = DL_NAME;

			break;
		case DL_PARAM: //отображаем дельту
			deep_level = DL_NAME;
			break;
		}
		break;
	case MD_DOWN: // движемс€ вниз
		switch (deep_level) { // провер€ем уровень глубины
		case DL_NAME: // переходим к дельте
			if (menuConfig->Menus[actualIndex].is_param) { // ≈сли параметр, погружаемс€ к дельте
				deep_level = DL_DELTA;
			} else { // »наче провер€ем чилдренов
				bool nochild = true;          // ‘лаг, есть ли дочерние элементы
				for (int i = 0; i < menuConfig->MenuSize; i++) {
					if (menuConfig->Menus[i].parent_id == actualIndex) {
						actualIndex = i; // ≈сли есть, делаем первый попавшийс€ актуальным элементом
						nochild = false;                         // ѕотомки есть
						break;                                 // ¬ыходим из for
					}
				}
				if (nochild) { // ≈сли же потомков нет, воспринимаем как команду
					menuConfig->ExecuteCallback(actualIndex); // » здесь обрабатываем по своему усмотрению
				}
			}
			break;
		case DL_DELTA: // пеоеходим к параметру
			deep_level = DL_PARAM;
			break;
		case DL_PARAM: // возвращаемс€ к дельте
			deep_level = DL_DELTA;
			break;
		}
		break;
	case MD_LEFT:
		switch (deep_level) { // провер€ем уровень глубины
		case DL_NAME: // это меню, ищем соседнее слева
			actualIndex = findNeighbor(menuConfig->Menus[actualIndex].parent_id,
					actualIndex, Direction);
			break;
		case DL_DELTA: // увеличиваем дельту
			tmpValue = menuConfig->Params[actualIndex].delta * 10;
			if (tmpValue <= menuConfig->Menus[actualIndex].d_max) {
				menuConfig->Params[actualIndex].delta = tmpValue;
			} else {
				menuConfig->Params[actualIndex].delta =
						menuConfig->Menus[actualIndex].d_min;
			}
			break;
		case DL_PARAM: // уменьшаем параметр
			tmpValue = menuConfig->Params[actualIndex].value
					- menuConfig->Params[actualIndex].delta;
			if ((tmpValue >= menuConfig->Menus[actualIndex].v_min)
					&& (tmpValue <= menuConfig->Menus[actualIndex].v_max)) {
				menuConfig->Params[actualIndex].value = tmpValue;
				menuConfig->ChangeCallback(actualIndex);
			}
			break;
		}
		break;
	case MD_RIGHT:
		switch (deep_level) { // провер€ем уровень глубины
		case DL_NAME: // это меню, ищем соседнее слева
			actualIndex = findNeighbor(menuConfig->Menus[actualIndex].parent_id,
					actualIndex, Direction);
			break;
		case DL_DELTA: // уменьшаем дельту
			tmpValue = menuConfig->Params[actualIndex].delta / 10;
			if (tmpValue >= menuConfig->Menus[actualIndex].d_min) {
				menuConfig->Params[actualIndex].delta = tmpValue;
			} else {
				menuConfig->Params[actualIndex].delta =
						menuConfig->Menus[actualIndex].d_max;
			}
			break;
		case DL_PARAM: // увеличиваем параметр
			tmpValue = menuConfig->Params[actualIndex].value
					+ menuConfig->Params[actualIndex].delta;
			if (tmpValue <= menuConfig->Menus[actualIndex].v_max) {
				menuConfig->Params[actualIndex].value = tmpValue;
				menuConfig->ChangeCallback(actualIndex);
			}
			break;
		}
		break;
	case MD_LONG:
		menuConfig->SaveCallback(actualIndex);
		break;
	case MD_NOTHING:
		break;
	}
	menuConfig->PrintCallback(actualIndex, deep_level);
	return 0;
}

int findNeighbor(int parentid, int id, TM_Direction side) { // ‘ункци€ получени€ индекса пункта меню следующего или предыдущего от актуального
	int i = id;
	if (side == MD_LEFT) { // бежим к началу, ищем узел с общим родителем
		do {
			i--;
			if (i == -1) { // переходим на конец массива
				i = menuConfig->MenuSize-1;
			}
			if (menuConfig->Menus[i].parent_id == parentid) { // встреча = выход
				break;
			}
		} while (i != id); // если узел такой один, возвращаем его же
	} else { //бежим к концу, ищем узел с общим родителем
		do {
			i++;
			if (i == menuConfig->MenuSize) { // переходим на начало массива
				i = 0;
			}
			if (menuConfig->Menus[i].parent_id == parentid) { // встреча = выход
				break;
			}
		} while (i != id);
	}
	return i;
}
