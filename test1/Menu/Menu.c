/*
 * Menu.c
 *
 *  Created on: 18.02.2014
 *      Author: estarcev
 */

#include "Menu.h"

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t NULL_MENU = {0};

/** \internal
 *  Pointer to the generic menu text display function
 *  callback, to display the configured text of a menu item
 *  if no menu-specific display function has been set
 *  in the select menu item.
 */
static void (*MenuWriteFunc)(const char* Text) = NULL;

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;
//static Menu_Item_t* tempMenu = NULL;

//static Menu_Item_t* MenuStack[10];

volatile uint8_t MenuStackTop;

tContext* gContext;

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

	if (MenuWriteFunc)
	{
		MenuWriteFunc(CurrentMenuItem->Text);
	}

	if(CurrentMenuItem->SelectCallback)
	{
		CurrentMenuItem->SelectCallback();
	}
}

void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char* Text))
{
	MenuWriteFunc = WriteFunc;
	Menu_Navigate(CurrentMenuItem);
}

void Menu_EnterCurrentItem(void)
{
	if ((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
	{
		return;
	}

	if(CurrentMenuItem->EnterCallback)
	{
		CurrentMenuItem->EnterCallback();
	}
}

/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void L1I1_Enter(void)
{
//	GrStringDraw(gContext, "ENTER", -1, 10, 10, 0);
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void L1I1_Select(void)
{
//	GrStringDraw(gContext, "SELECT", -1, 100, 10, 0);
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
//static void Generic_Write(const char* Text)
//{
//	if (Text)
//	{
//		GrStringDraw(gContext, Text, -1, 100, 10, 0);
//	}
//}


//		  Name, 		Next, 		Previous, 	Parent, 		Child, 			SelectFunc, 		EnterFunc, 			Text
MENU_ITEM(Menu_1, 		Menu_2, 	Menu_3, 	NULL_MENU, 		Menu_1_1,  		L1I1_Select, 				L1I1_Enter, 				"время");
MENU_ITEM(Menu_2, 		Menu_3, 	Menu_1, 	NULL_MENU, 		NULL_MENU, 		L1I1_Select, 				L1I1_Enter, 				"скорость");
MENU_ITEM(Menu_3, 		Menu_1, 	Menu_2, 	NULL_MENU, 		NULL_MENU, 		L1I1_Select, 				L1I1_Enter, 				"уровень");

MENU_ITEM(Menu_1_1, 	Menu_1_2, 	Menu_1_2, 	NULL_MENU, 		NULL_MENU, 		L1I1_Select, 		L1I1_Enter, 		"настроить");
MENU_ITEM(Menu_1_2, 	Menu_1_1, 	Menu_1_1, 	NULL_MENU, 		NULL_MENU, 		L1I1_Select, 		L1I1_Enter, 		"сбросить");


void DrawMenu(Menu_Item_t* mi, uint8_t level)
{
	if(mi)
	{
		GrStringDraw(gContext, mi->Text, -1, 10, level, 0);
		if(CurrentMenuItem == mi)
		{
			GrStringDraw(gContext, ">", -1, 0, level, 0);
		}
		else
		{
			GrStringDraw(gContext, "  ", -1, 0, level, 0);
		}
	}
	if(mi->Next && mi->Next !=  &Menu_1)
	{
		DrawMenu(mi->Next, level + 30);
	}

}

void MenuInitialize(tContext* context)
{
	gContext = context;

	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
//	Menu_SetGenericWriteCallback(Generic_Write);
	Menu_Navigate(&Menu_1);
	DrawMenu(&Menu_1, 0);
}



void ProcessMenu()
{
	while(true)
	{

	}
}
