/***********************************************************************
* COSC1076 - Advanced Programming Techniques
* Semester 2 2015 Assignment #2
* Full Name        : KIROLOS KALDAS
* Student Number   : s3545643
* Course Code      : COSC1076
* Program Code     : BP096
* Start up code provided by Paul Miller
* Some codes are adopted here with permission by an anonymous author
***********************************************************************/

#include "ppd_menu.h"
/**
* @file ppd_menu.c handles the initialised and management of the menu
* array
**/

/**
* @param menu the menu item array to initialise
**/
void init_menu(struct menu_item * menu)
{
	int i;

	/* The function names */
	char * menu_titles[MENU_OPTIONS] =
	{
		"Display Items",
		"Purchase Items",
		"Save and Exit",
		"Add Item",
		"Remove Item",
		"Display Coins",
		"Reset Stock",
		"Reset Coins"
	};

	/* the function pointers */
	BOOLEAN(*menu_functions[MENU_OPTIONS])(struct ppd_system*) =
	{
		display_items,
		purchase_item,
		save_system,
		add_item,
		remove_item,
		display_coins,
		reset_stock,
		reset_coins
	};

	for (i = 0; i < MENU_OPTIONS; i++)
	{
		strcpy(menu[i].name, menu_titles[i]);
		menu[i].function = menu_functions[i];
	}
}

/* this function prints out the menu */
void display_menu(struct menu_item * menu)
{
	int i;

	printf("\nMain Menu:\n");
	for (i = 0; i < MENU_OPTIONS; i++)
	{
		printf("\t%d. %s\n", i + 1, menu[i].name);

		if (i == 2)
		{
			printf("Administrator-Only Menu:\n");
		}
	}
	printf("\t9. Abort Program\n");
}
