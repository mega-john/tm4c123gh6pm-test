/*
 * Menu.c
 *
 *  Created on: 18.02.2014
 *      Author: estarcev
 */

#include <stdlib.h>
#include "../global.h"

Menu_Item_t NULL_MENU = { 0 };
Menu_Item_t* CurrentMenuItem = &NULL_MENU;
static volatile bool updateMenu = false;

//                  Name, IsFirst, IsLast, Next, Previous, Parent, Child, SelectFunc, EnterFunc, Text
MAKE_MENU(Menu_1, true, false, Menu_2, Menu_4, NULL_MENU, Menu_1_1, L1I1_Select, L1I1_Enter, "�����");
MAKE_MENU(Menu_1_1, true, false, Menu_1_2, Menu_1_2, Menu_1, NULL_MENU,L1I1_Select, L1I1_Enter, "���������");
MAKE_MENU(Menu_1_2, false, true, Menu_1_1, Menu_1_1, Menu_1, NULL_MENU,L1I1_Select, L1I1_Enter, "��������");

MAKE_MENU(Menu_2, false, false, Menu_3, Menu_1, NULL_MENU, Menu_2_1,    L1I1_Select, L1I1_Enter, "��������");
MAKE_MENU(Menu_2_1, true, false, Menu_2_2, Menu_2_2, Menu_2, NULL_MENU,L1I1_Select, L1I1_Enter, "���������");
MAKE_MENU(Menu_2_2, false, true, Menu_2_1, Menu_2_1, Menu_2, NULL_MENU,L1I1_Select, L1I1_Enter, "��������");

MAKE_MENU(Menu_3, false, false, Menu_4, Menu_2, NULL_MENU, Menu_3_1,L1I1_Select, L1I1_Enter, "��������");
MAKE_MENU(Menu_3_1, true, false, Menu_3_2, Menu_3_2, Menu_3, NULL_MENU,L1I1_Select, L1I1_Enter, "���������");
MAKE_MENU(Menu_3_2, false, true, Menu_3_1, Menu_3_1, Menu_3, NULL_MENU,L1I1_Select, L1I1_Enter, "��������");

MAKE_MENU(Menu_4, false, true, Menu_1, Menu_3, NULL_MENU, Menu_4_1, L1I1_Select,L1I1_Enter, "�����������");
MAKE_MENU(Menu_4_1, true, false, Menu_4_2, Menu_4_3, Menu_4, NULL_MENU, L1I1_Select, L1I1_Enter, "���������");
MAKE_MENU(Menu_4_2, false, false, Menu_4_3, Menu_4_1, Menu_4, NULL_MENU,        L1I1_Select, L1I1_Enter, "��������");
MAKE_MENU(Menu_4_3, false, true, Menu_4_1, Menu_4_2, Menu_4, Menu_4_3_1,        L1I1_Select, L1I1_Enter, "�������� �.");
MAKE_MENU(Menu_4_3_1, true, false, Menu_4_3_2, Menu_4_3_1, Menu_4_3, NULL_MENU,     L1I1_Select, L1I1_Enter, "�����");
MAKE_MENU(Menu_4_3_2, false, true, Menu_4_3_1, Menu_4_3_2, Menu_4_3, NULL_MENU,     L1I1_Select, L1I1_Enter, "�����");

MAKE_MENU(MainMenu_1, true, false, MainMenu_2, MainMenu_5, NULL_MENU, NULL_MENU,        MainMenu_1_select, MainMenu_1_enter, "������");
MAKE_MENU(MainMenu_2, false, false, MainMenu_3, MainMenu_1, NULL_MENU,      NULL_MENU, MainMenu_2_select, MainMenu_2_enter, "����� ����");
MAKE_MENU(MainMenu_3, false, false, MainMenu_4, MainMenu_3, NULL_MENU,      NULL_MENU, MainMenu_3_select, MainMenu_3_enter, "������");
MAKE_MENU(MainMenu_4, false, false, MainMenu_5, MainMenu_3, NULL_MENU,      NULL_MENU, MainMenu_4_select, MainMenu_4_enter, "��������");
MAKE_MENU(MainMenu_5, true, true, MainMenu_1, MainMenu_4, NULL_MENU, NULL_MENU,     MainMenu_5_select, MainMenu_5_enter, "����� ��������");

uint8_t owDevicesIDs[MAX_OW_DEVICES][8];
uint8_t themperature[MAX_OW_DEVICES][3];

void ClearScreen()
{
	GrContextForegroundSet(&g_sContext, BACKGROUND);
	uint16_t i = 0;
	for (; i < g_sContext.psDisplay->ui16Height; i++)
	{
		GrLineDrawH(&g_sContext, 0, g_sContext.psDisplay->ui16Width, i);
	}
}

void ClearClientArea(tRectangle* area)
{

}

void DrawTemperature(void * params)
{
	tRectangle r;
	r.i16XMin = 10;
	r.i16XMax = 234;
	r.i16YMin = 280;
	r.i16YMax = r.i16YMin + GrStringHeightGet(&g_sContext);

	GrContextForegroundSet(&g_sContext, BACKGROUND);
    char tmp[30] = "value";
    char buffer[sizeof(int) * 8 + 1];
    ltoa(themperature[0][2] * 625, buffer);
    buffer[4] = '\0';
    usprintf(tmp, "value: %c%d.%s C", themperature[0][0], themperature[0][1], buffer);
//    usprintf(tmp, "value: %c%d.%d C", themperature[0][0], themperature[0][1], themperature[0][2]);
	GrRectFill(&g_sContext, &r);
	GrContextForegroundSet(&g_sContext, FOREGROUND);
	GrStringDraw(&g_sContext, tmp, 29, 10, 280, 1);
}

void MenuNavigate(Menu_Item_t* const NewMenu)
{
	if ((NewMenu == &NULL_MENU) || (NewMenu == NULL))
	{
		updateMenu = true;
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



void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t color)
{
	GrLineDraw(&g_sContext, x1, y1, x2, y2);
	GrLineDraw(&g_sContext, x1, y1, x3, y3);
	GrLineDraw(&g_sContext, x2, y2, x3, y3);
}

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
	r.i16XMax = 234;
	r.i16YMin = offset;
	r.i16YMax = r.i16YMin + GrStringHeightGet(&g_sContext);

	GrContextForegroundSet(&g_sContext, isSelect ? FOREGROUND: BACKGROUND);
	GrRectDraw(&g_sContext, &r);
	GrCircleFill(&g_sContext, 220, offset + GrStringHeightGet(&g_sContext) / 2, 5);
}

void DrawMenu()
{
	uint16_t offsetStep = (GrStringHeightGet(&g_sContext) + 1);
	uint16_t offset = offsetStep; // + 5;
	Menu_Item_t* firstElement = GetFirstMenuElement();

	GrContextForegroundSet(&g_sContext, ClrWhite);

	if (firstElement->Parent == &NULL_MENU)
	{
		GrStringDrawCentered(&g_sContext, "��c������", -1, g_sContext.psDisplay->ui16Width / 2,
				GrStringHeightGet(&g_sContext) / 2, 1);
		GrLineDrawH(&g_sContext, 1, g_sContext.psDisplay->ui16Width, offset);
//		GrLineDrawH(&g_sContext, 1, g_sContext.psDisplay->ui16Width, ++offset);
	}
	else
	{
		GrStringDrawCentered(&g_sContext, firstElement->Parent->Text, -1,
		        g_sContext.psDisplay->ui16Width / 2, GrStringHeightGet(&g_sContext) / 2, 1);
		GrLineDrawH(&g_sContext, 1, g_sContext.psDisplay->ui16Width, offset);
//		GrLineDrawH(&g_sContext, 1, g_sContext.psDisplay->ui16Width, ++offset);
	}
	offset += 5;
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

void MenuInitialize()
{
	MenuNavigate(&Menu_1);
}

extern update_flags flags;

void ProcessMenu()
{
	tRectangle sRect;
	sRect.i16XMin = 0;
	sRect.i16YMin = 0;
	sRect.i16XMax = g_sContext.psDisplay->ui16Width;
	sRect.i16YMax = g_sContext.psDisplay->ui16Height;

	if (updateMenu)
	{
		GrContextForegroundSet(&g_sContext, FOREGROUND);
		GrRectDraw(&g_sContext, &sRect);

		DrawMenu();
		updateMenu = false;
	}
	if(flags.update_menu)
	{
        DrawTemperature(NULL);
        flags.update_menu=false;
	}
}
