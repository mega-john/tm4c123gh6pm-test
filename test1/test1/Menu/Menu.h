#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "grlib/grlib.h"
#include "../hardware/ili9341/ili9341.h"

extern void ProcessMenu();
extern void MenuInitialize(/*tContext* context*/);
extern void ClearScreen();
extern void ClearClientArea(tRectangle* area);

typedef const struct MI
{
	const bool isFirst;//first element flag
	const bool isLast;//last element flag
	const struct MI *Next; /**< Pointer to the next menu item of this menu item */
	const struct MI *Previous; /**< Pointer to the previous menu item of this menu item */
	const struct MI *Parent; /**< Pointer to the parent menu item of this menu item */
	const struct MI *Child; /**< Pointer to the child menu item of this menu item */
	void (*SelectCallback)(void); /**< Pointer to the optional menu-specific select callback of this menu item */
	void (*EnterCallback)(void); /**< Pointer to the optional menu-specific enter callback of this menu item */
	const char Text[20]; /**< Menu item text to pass to the menu display callback function */
} Menu_Item_t;

extern Menu_Item_t NULL_MENU;
extern Menu_Item_t* CurrentMenuItem;

/** Creates a new menu item entry with the specified links and callbacks.
 *
 *  \param[in] Name      Name of the menu entry, must be unique.
 *  \param[in] IsFirst	 Is first elemnt of menu
 *  \param[in] IsLast	 Is last elemnt of menu
 *  \param[in] Next      Name of the next linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Previous  Name of the previous linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Parent    Name of the parent linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Child     Name of the child linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] SelectFunc  Function callback to execute when the menu item is selected, or \c NULL for no callback.
 *  \param[in] EnterFunc   Function callback to execute when the menu item is entered, or \c NULL for no callback.
 */
#define MAKE_MENU(Name, IsFirst, IsLast, Next, Previous, Parent, Child, SelectFunc, EnterFunc, Text) \
	extern Menu_Item_t Next;     	\
	extern Menu_Item_t Previous; 	\
	extern Menu_Item_t Parent;   	\
	extern Menu_Item_t Child;  		\
	Menu_Item_t Name = {IsFirst, IsLast, &Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, Text}

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu parent. */
#define MENU_PARENT         (CurrentMenuItem->Parent)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
#define MENU_CHILD          (CurrentMenuItem->Child)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
#define MENU_NEXT           (CurrentMenuItem->Next)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the previous linked menu item. */
#define MENU_PREVIOUS       (CurrentMenuItem->Previous)

void MenuNavigate(Menu_Item_t* const NewMenu);

///////////////////////////////////////////////////////////////////////////
//function prototypes
///////////////////////////////////////////////////////////////////////////
extern void MainMenu_1_select();
extern void MainMenu_1_enter();
extern void MainMenu_2_select();
extern void MainMenu_2_enter();
extern void MainMenu_3_select();
extern void MainMenu_3_enter();
extern void MainMenu_4_select();
extern void MainMenu_4_enter();
extern void MainMenu_5_select();
extern void MainMenu_5_enter();


void DrawTemperature(void * params);
