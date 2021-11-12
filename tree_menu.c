/*
 * tree_menu.c
 *
 *  Created on: 22 ���. 2020 �.
 *      Author: Maksim Cherkasov
 */

#include "tree_menu.h"

TM_Config* menuConfig;
bool init = false;

int actualIndex = 0;
TM_DeepLevel deep_level = DL_NAME; // ������� ���������� 0 - ��� ���������, 1 - ������, 2 - ��������
TM_ParamType tmpValue = 0; // ��������� ���������� ��� �������� ����������� ���������

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
	case MD_UP: // �������� �����
		switch (deep_level) { // ��������� ������� �������
		case DL_NAME: //������� �� ��������
			if (menuConfig->Menus[actualIndex].parent_id >= 0) { // ���� ���� ���� ������������ ����� (ParentID>0)
				actualIndex = menuConfig->Menus[actualIndex].parent_id;
			}
			break;
		case DL_DELTA: //���������� ���
			deep_level = DL_NAME;

			break;
		case DL_PARAM: //���������� ������
			deep_level = DL_NAME;
			break;
		}
		break;
	case MD_DOWN: // �������� ����
		switch (deep_level) { // ��������� ������� �������
		case DL_NAME: // ��������� � ������
			if (menuConfig->Menus[actualIndex].is_param) { // ���� ��������, ����������� � ������
				deep_level = DL_DELTA;
			} else { // ����� ��������� ���������
				bool nochild = true;          // ����, ���� �� �������� ��������
				for (int i = 0; i < menuConfig->MenuSize; i++) {
					if (menuConfig->Menus[i].parent_id == actualIndex) {
						actualIndex = i; // ���� ����, ������ ������ ���������� ���������� ���������
						nochild = false;                         // ������� ����
						break;                                 // ������� �� for
					}
				}
				if (nochild) { // ���� �� �������� ���, ������������ ��� �������
					menuConfig->ExecuteCallback(actualIndex); // � ����� ������������ �� ������ ����������
				}
			}
			break;
		case DL_DELTA: // ��������� � ���������
			deep_level = DL_PARAM;
			break;
		case DL_PARAM: // ������������ � ������
			deep_level = DL_DELTA;
			break;
		}
		break;
	case MD_LEFT:
		switch (deep_level) { // ��������� ������� �������
		case DL_NAME: // ��� ����, ���� �������� �����
			actualIndex = findNeighbor(menuConfig->Menus[actualIndex].parent_id,
					actualIndex, Direction);
			break;
		case DL_DELTA: // ����������� ������
			tmpValue = menuConfig->Params[actualIndex].delta * 10;
			if (tmpValue <= menuConfig->Menus[actualIndex].d_max) {
				menuConfig->Params[actualIndex].delta = tmpValue;
			} else {
				menuConfig->Params[actualIndex].delta =
						menuConfig->Menus[actualIndex].d_min;
			}
			break;
		case DL_PARAM: // ��������� ��������
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
		switch (deep_level) { // ��������� ������� �������
		case DL_NAME: // ��� ����, ���� �������� �����
			actualIndex = findNeighbor(menuConfig->Menus[actualIndex].parent_id,
					actualIndex, Direction);
			break;
		case DL_DELTA: // ��������� ������
			tmpValue = menuConfig->Params[actualIndex].delta / 10;
			if (tmpValue >= menuConfig->Menus[actualIndex].d_min) {
				menuConfig->Params[actualIndex].delta = tmpValue;
			} else {
				menuConfig->Params[actualIndex].delta =
						menuConfig->Menus[actualIndex].d_max;
			}
			break;
		case DL_PARAM: // ����������� ��������
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

int findNeighbor(int parentid, int id, TM_Direction side) { // ������� ��������� ������� ������ ���� ���������� ��� ����������� �� �����������
	int i = id;
	if (side == MD_LEFT) { // ����� � ������, ���� ���� � ����� ���������
		do {
			i--;
			if (i == -1) { // ��������� �� ����� �������
				i = menuConfig->MenuSize-1;
			}
			if (menuConfig->Menus[i].parent_id == parentid) { // ������� = �����
				break;
			}
		} while (i != id); // ���� ���� ����� ����, ���������� ��� ��
	} else { //����� � �����, ���� ���� � ����� ���������
		do {
			i++;
			if (i == menuConfig->MenuSize) { // ��������� �� ������ �������
				i = 0;
			}
			if (menuConfig->Menus[i].parent_id == parentid) { // ������� = �����
				break;
			}
		} while (i != id);
	}
	return i;
}
