/*
 * tree_menu.h
 *
 *  Created on: 22 θών. 2020 γ.
 *      Author: Maksim Cherkasov
 */

#ifndef INC_TREE_MENU_H_
#define INC_TREE_MENU_H_

//#include "stm32f0xx_hal.h"

#ifndef __cplusplus
typedef enum {
	false, true
} bool;
#endif

typedef unsigned int TM_ParamType;

typedef enum {
	MD_UP, MD_DOWN, MD_LEFT, MD_RIGHT, MD_LONG, MD_NOTHING
} TM_Direction;

typedef enum {
	DL_NAME, DL_DELTA, DL_PARAM
} TM_DeepLevel;

typedef struct {
	TM_ParamType value;
	TM_ParamType delta;
} TM_Param;

typedef struct {
//	signed char id;
	signed char parent_id;
	bool is_param;
	char name[15];
	TM_ParamType v_min;
	TM_ParamType v_max;
	TM_ParamType d_min;
	TM_ParamType d_max;
} TM_Unit;

typedef struct {
	int MenuSize;
	const TM_Unit* Menus;
	TM_Param* Params;
	void (*PrintCallback)(int index, TM_DeepLevel DeepLevel);
	void (*ChangeCallback)(int index);
	void (*SaveCallback)(int index);
	void (*ExecuteCallback)(int index);
} TM_Config;

void InitMenu(TM_Config* Configuration);
int JumpMenu(TM_Direction Direction);

#endif /* INC_TREE_MENU_H_ */
