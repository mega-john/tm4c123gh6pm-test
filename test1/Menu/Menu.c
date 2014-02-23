/*
 * Menu.c
 *
 *  Created on: 18.02.2014
 *      Author: estarcev
 */

#include <stdlib.h>
#include "Menu.h"

Menu_Item_t NULL_MENU = { 0 };

static volatile bool updateMenu = false;
static volatile Menu_Item_t* CurrentMenuItem = &NULL_MENU;
static volatile uint8_t MenuStackTop;

tContext* gContext;

void ClearScreen()
{
	GrContextForegroundSet(gContext, ClrDarkBlue);
	uint16_t i = 0;
	for(; i < DISPLAY_HEIGHT; i++)
	{
		GrLineDrawH(gContext, 0, DISPLAY_WIDTH, i);
	}
}

Menu_Item_t* Menu_GetCurrentMenu(void)
{
	return CurrentMenuItem;
}

void Menu_Navigate(Menu_Item_t* const NewMenu)
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
MENU_ITEM(Menu_1, true, false, Menu_2, Menu_3, NULL_MENU, Menu_1_1, L1I1_Select, L1I1_Enter, "время");
	MENU_ITEM(Menu_1_1, true, false, Menu_1_2, Menu_1_2, Menu_1, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MENU_ITEM(Menu_1_2, false, true, Menu_1_1, Menu_1_1, Menu_1, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");

MENU_ITEM(Menu_2, false, false, Menu_3, Menu_1, NULL_MENU, Menu_2_1, L1I1_Select, L1I1_Enter, "скорость");
	MENU_ITEM(Menu_2_1, true, false, Menu_2_2, Menu_2_2, Menu_2, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MENU_ITEM(Menu_2_2, false, true, Menu_2_1, Menu_2_1, Menu_2, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");

MENU_ITEM(Menu_3, false, false, Menu_4, Menu_2, NULL_MENU, Menu_3_1, 	L1I1_Select, L1I1_Enter, "бензобак");
	MENU_ITEM(Menu_3_1, true, false, Menu_3_2, Menu_3_2, Menu_3, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MENU_ITEM(Menu_3_2, false, true, Menu_3_1, Menu_3_1, Menu_3, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");

MENU_ITEM(Menu_4, false, true, Menu_1, Menu_3, NULL_MENU, Menu_4_1, 	L1I1_Select, L1I1_Enter, "температура");
	MENU_ITEM(Menu_4_1, true, 	false, 	Menu_4_2, Menu_4_3, Menu_4, NULL_MENU, L1I1_Select, L1I1_Enter, "настроить");
	MENU_ITEM(Menu_4_2, false, false,		Menu_4_3, Menu_4_1, Menu_4, NULL_MENU, L1I1_Select, L1I1_Enter, "сбросить");
	MENU_ITEM(Menu_4_3, false, true, 		Menu_4_1, Menu_4_2, Menu_4, Menu_4_3_1, L1I1_Select, L1I1_Enter, "показать д.");
		MENU_ITEM(Menu_4_3_1, true, false, Menu_4_3_2, Menu_4_3_2, Menu_4_3, NULL_MENU, L1I1_Select, L1I1_Enter, "салон");
		MENU_ITEM(Menu_4_3_2, false, true, Menu_4_3_2, Menu_4_3_2, Menu_4_3, NULL_MENU, L1I1_Select, L1I1_Enter, "улица");

Menu_Item_t* GetFirstMenuElement()
{
	Menu_Item_t* firstElement = Menu_GetCurrentMenu();
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
	r.i16YMin = offset + 3;
	r.i16YMax = r.i16YMin + GrStringHeightGet(gContext);

	GrContextForegroundSet(gContext, isSelect ? ClrYellow: ClrDarkBlue);
	GrRectDraw(gContext, &r);
	GrCircleFill(gContext, 220, offset + 22, 5);
}

void DrawMenu()
{
	uint16_t offsetStep = (GrStringHeightGet(gContext) + 5);
	uint16_t offset = offsetStep;
	Menu_Item_t* firstElement = GetFirstMenuElement();

	GrContextForegroundSet(gContext, ClrWhite);

	if (firstElement->Parent == &NULL_MENU)
	{
		GrStringDrawCentered(gContext, "Наcтройки", -1, 120, 20, 0);
		GrLineDrawH(gContext, 1, DISPLAY_WIDTH, offset);
		GrLineDrawH(gContext, 1, DISPLAY_WIDTH, ++offset);
	}
	else
	{
		GrStringDrawCentered(gContext, firstElement->Parent->Text, -1, 120, 20, 0);
		GrLineDrawH(gContext, 1, DISPLAY_WIDTH, offset);
		GrLineDrawH(gContext, 1, DISPLAY_WIDTH, ++offset);
	}

	do
	{
		if (firstElement == Menu_GetCurrentMenu())
		{
			DrawSelection(offset, true);
		}
		else
		{
			DrawSelection(offset, false);
		}
		GrContextForegroundSet(gContext, ClrWhite);
		GrStringDraw(gContext, firstElement->Text, -1, 10, offset, 0);
		if (firstElement->isLast)
		{
			break;
		}
		firstElement = firstElement->Next;
		offset += offsetStep;
	} while (true);
}

void MenuInitialize(tContext* context)
{
	gContext = context;
	Menu_Navigate(&Menu_1);
}

void ProcessMenu()
{
	if (updateMenu)
	{
		DrawMenu();
		updateMenu = false;
	}
}
