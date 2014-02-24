/*
 * Menu.c
 *
 *  Created on: 18.02.2014
 *      Author: estarcev
 */

#include "../global.h"

Menu_Item_t NULL_MENU = { 0 };
Menu_Item_t* CurrentMenuItem = &NULL_MENU;
static volatile bool updateMenu = false;

void ClearScreen()
{
	GrContextForegroundSet(&g_sContext, ClrDarkBlue);
	uint16_t i = 0;
	for(; i < DISPLAY_HEIGHT; i++)
	{
		GrLineDrawH(&g_sContext, 0, DISPLAY_WIDTH, i);
	}
}

void ClearClientArea(tRectangle* area)
{

}

void MenuNavigate(Menu_Item_t* const NewMenu)
{
	if ((NewMenu == &NULL_MENU) || (NewMenu == NULL))
	{
		return;
	}

	CurrentMenuItem = NewMenu;

	if (CurrentMenuItem->SelectCallback)
	{
		CurrentMenuItem->SelectCallback();
	}
	updateMenu = true;
}

void L1I1_Enter(void)
{
//	GrStringDraw(gContext, "ENTER", -1, 10, 10, 0);
}

void L1I1_Select(void)
{
//	GrStringDraw(gContext, "SELECT", -1, 100, 10, 0);
}

//					Name, IsFirst, IsLast, Next, Previous, Parent, Child, SelectFunc, EnterFunc, Text
MAKE_MENU(Menu_1, true, false, Menu_2, Menu_3, NULL_MENU, Menu_1_1, L1I1_Select, L1I1_Enter, "ВРЕМЯ");
	MAKE_MENU(Menu_1_1, true, false, Menu_1_2, Menu_1_2, Menu_1, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MAKE_MENU(Menu_1_2, false, true, Menu_1_1, Menu_1_1, Menu_1, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");

MAKE_MENU(Menu_2, false, false, Menu_3, Menu_1, NULL_MENU, Menu_2_1, L1I1_Select, L1I1_Enter, "СКОРОСТЬ");
	MAKE_MENU(Menu_2_1, true, false, Menu_2_2, Menu_2_2, Menu_2, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MAKE_MENU(Menu_2_2, false, true, Menu_2_1, Menu_2_1, Menu_2, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");

MAKE_MENU(Menu_3, false, false, Menu_4, Menu_2, NULL_MENU, Menu_3_1, 	L1I1_Select, L1I1_Enter, "БЕНЗОБАК");
	MAKE_MENU(Menu_3_1, true, false, Menu_3_2, Menu_3_2, Menu_3, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MAKE_MENU(Menu_3_2, false, true, Menu_3_1, Menu_3_1, Menu_3, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");

MAKE_MENU(Menu_4, false, true, Menu_1, Menu_3, NULL_MENU, Menu_4_1, 	L1I1_Select, L1I1_Enter, "ТЕМПЕРАТУРА");
	MAKE_MENU(Menu_4_1, true, 	false, 	Menu_4_2, Menu_4_3, Menu_4, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MAKE_MENU(Menu_4_2, false, false,		Menu_4_3, Menu_4_1, Menu_4, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");
	MAKE_MENU(Menu_4_3, false, true, 		Menu_4_1, Menu_4_2, Menu_4, Menu_4_3_1, L1I1_Select, L1I1_Enter, "показать д.");
		MAKE_MENU(Menu_4_3_1, true, false, Menu_4_3_2, Menu_4_3_1, Menu_4_3, NULL_MENU, L1I1_Select, L1I1_Enter, "салон");
		MAKE_MENU(Menu_4_3_2, false, true, Menu_4_3_1, Menu_4_3_2, Menu_4_3, NULL_MENU, L1I1_Select, L1I1_Enter, "улица");


void MainMenu_1_select();
void MainMenu_1_enter();
void MainMenu_2_select();
void MainMenu_2_enter();
void MainMenu_3_select();
void MainMenu_3_enter();
void MainMenu_4_select();
void MainMenu_4_enter();
void MainMenu_5_select();
void MainMenu_5_enter();


MAKE_MENU(MainMenu_1, true, false, MainMenu_2, MainMenu_5, NULL_MENU, NULL_MENU, 	MainMenu_1_select, MainMenu_1_enter, "Расход");
MAKE_MENU(MainMenu_2, false, false, MainMenu_3, MainMenu_1, NULL_MENU, NULL_MENU, 	MainMenu_2_select, MainMenu_2_enter, "Запас хода");
MAKE_MENU(MainMenu_3, false, false, MainMenu_4, MainMenu_3, NULL_MENU, NULL_MENU, 	MainMenu_3_select, MainMenu_3_enter, "Пробег");
MAKE_MENU(MainMenu_4, false, false, MainMenu_5, MainMenu_3, NULL_MENU, NULL_MENU, 	MainMenu_4_select, MainMenu_4_enter, "Скорость");
MAKE_MENU(MainMenu_5, true, true, MainMenu_1, MainMenu_4, NULL_MENU, NULL_MENU, 	MainMenu_5_select, MainMenu_5_enter, "Время движения");




Menu_Item_t* GetFirstMenuElement()
{
	Menu_Item_t* firstElement = CurrentMenuItem;
	do
	{
		if (firstElement->isFirst)
		{
			break;
		}
		else
		{
			firstElement = firstElement->Next;
		}
	} while (true);

	return firstElement;
}

void DrawSelection(uint16_t offset, bool isSelect)
{
	tRectangle r;
	r.i16XMin = 5;
	r.i16XMax = 235;
	r.i16YMin = offset;
	r.i16YMax = r.i16YMin + GrStringHeightGet(&g_sContext) + 1;

	GrContextForegroundSet(&g_sContext, isSelect ? ClrYellow: ClrDarkBlue);
	GrRectDraw(&g_sContext, &r);
	GrCircleFill(&g_sContext, 220, offset + GrStringHeightGet(&g_sContext) / 2 + 3, 5);
}

void DrawMenu()
{
	uint16_t offsetStep = (GrStringHeightGet(&g_sContext) + 5);
	uint16_t offset = offsetStep + 5;
	Menu_Item_t* firstElement = GetFirstMenuElement();

	GrContextForegroundSet(&g_sContext, ClrWhite);

	if (firstElement->Parent == &NULL_MENU)
	{
		GrStringDrawCentered(&g_sContext, "Наcтройки", -1, DISPLAY_WIDTH / 2, GrStringHeightGet(&g_sContext) / 2, 1);
		GrLineDrawH(&g_sContext, 1, DISPLAY_WIDTH, offset);
		GrLineDrawH(&g_sContext, 1, DISPLAY_WIDTH, ++offset);
	}
	else
	{
		GrStringDrawCentered(&g_sContext, firstElement->Parent->Text, -1, DISPLAY_WIDTH / 2, GrStringHeightGet(&g_sContext) / 2, 1);
		GrLineDrawH(&g_sContext, 1, DISPLAY_WIDTH, offset);
		GrLineDrawH(&g_sContext, 1, DISPLAY_WIDTH, ++offset);
	}

	do
	{
		if (firstElement == CurrentMenuItem)
		{
			DrawSelection(offset, true);
		}
		else
		{
			DrawSelection(offset, false);
		}
		GrContextForegroundSet(&g_sContext, ClrWhite);
		GrStringDraw(&g_sContext, firstElement->Text, -1, 10, offset, 0);
		if (firstElement->isLast)
		{
			break;
		}
		firstElement = firstElement->Next;
		offset += offsetStep;
	} while (true);
}

void MenuInitialize(/*tContext* context*/)
{
//	gContext = context;
	MenuNavigate(&Menu_1);
}

void ProcessMenu()
{
	if (updateMenu)
	{
		DrawMenu();
		updateMenu = false;
	}
}
