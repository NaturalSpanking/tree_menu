# tree_menu
Tree menus library for MCUs.

Библиотека для работы с древовидными меню в микроконтроллерах.

Работает в паре с [генератором](https://github.com/NaturalSpanking/TreeMenuGen) древовидных меню.
# Содержание
- [Подключение](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Описание дерева](#tree)


<a id="install"></a>
## Подключение
1. Скопировать файлы `.h` и `.c` в каталог с проектом.
2. Добавить `#include "tree_menu.h"` в файл `main.c`.
3. Выполнить инициализацию.

<a id="init"></a>
## Инициализация
1. Создать и заполнить массив узлов.
```c
#define TM_MENU_SIZE 6
const TM_Unit menu[TM_MENU_SIZE]={
  {-1, true, "Menu0", 0, 100, 1, 100},
  {-1, false, "Menu1", 0, 100, 1, 100},
    {1, true, "Menu4", 0, 100, 1, 100},
    {1, true, "Menu5", 0, 100, 1, 100},
  {-1, true, "Menu2", 0, 100, 1, 100},
  {-1, true, "Menu3", 0, 100, 1, 100}
};
```
2. Создать и заполнить массив параметров.
```c
TM_Param params[TM_MENU_SIZE]={
  {0, 10},{0, 10},{0, 10},{0, 10},{0, 10},
  {0, 10}
};
```
3. Создать прототипы функций обратных вызовов (печать меню, изменение параметра, сохранение параметра, выполнение команды).
```c
void PrintMenu(int index,TM_DeepLevel DeepLevel){
  // printing function prototype
  switch(DeepLevel){}
}
void ChangeParam(int index){
  // change parameter function prototype
  switch(index){}
}
void SaveParam(int index){
  // save parameter function prototype
  switch(index){}
}
void ExecuteCmd(int index){
  // Сommand execution function prototype
  switch(index){}
}
```
4. Создать и заполнить структуку инициализации.
```c
TM_Config MConfig;
MConfig.MenuSize = TM_MENU_SIZE;
MConfig.Menus = menu;
MConfig.Params = params;
MConfig.PrintCallback = PrintMenu;
MConfig.ChangeCallback = ChangeParam;
MConfig.ExecuteCallback = ExecuteCmd;
MConfig.SaveCallback = SaveParam;
```
5. Вызвать функцию инициализации.
```c
InitMenu(&MConfig);
```
Пункты 1-3 можно выполнить с помощью генератора меню (см. выше).

<a id="usage"></a>
## Использование
```c
switch (CheckButtons()) {
	case BTN_1:
		JumpMenu(MD_LEFT);
	break;
	case BTN_3:
		JumpMenu(MD_RIGHT);
	break;
	case BTN_2:
		JumpMenu(MD_DOWN);
	break;
	case BTN_4:
		JumpMenu(MD_UP);
	break;
	case BTN_5:
    JumpMenu(MD_LONG); 
	break;
}
```
<a id="tree"></a>
## Описание дерева
Каждый узел в дереве может быть:
- Подменю - включает в себя другие узлы, при переходе первый дочерний узел становится основным;
- Командой - не содержит значений, при переходе вызывается обработчик команды;
- Параметром - содержит изменяемое (поразрадно) значение, при переходе выбирается разряд для редактирования.

Каждый узел содержит поля:
- `id` - идентификатор узла, индекс в выходном массиве;
- `parent_id` - идентификатор родителя, индекс родителя в выходном массива;
- `Is parameter` - признак параметра, не может быть `true` для подменю;
- `Name` - имя узла;
- `Init value` - начальное значение;
- `Min value` - минимальное значение;
- `Max value` - максимальное значение;
- `Init delta` - начальный разряд для изменения;
- `Min delta` - минимальный разряд;
- `Max delta` - максимальный разряд;

Поля `value` и `delta` актуальны только для параметров.

Переходы по дереву:
- Влево\вправо - смена узла, выбор разряда или изменение значения разряда;
- Вперед\назад - смена узла, выполнение команды, переход к выбору разряда, или переход к изменению значения.
- Доп. инструмент - дополнительная команда, например сохранения в EEPROM
