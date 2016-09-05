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

#include "ppd_utility.h"

/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system, const char * coins_name,
	const char * stock_name)
{
	/* loads the data of from the stock file */
	if (load_stock(stock_name, system) == FALSE)
	{
		return FALSE;
	}

	/* loads the data from the coins file */
	if (load_coins(coins_name, system) == FALSE)
	{
		return FALSE;
	}

	system->stock_file_name = stock_name;
	system->coin_file_name = coins_name;

	return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
	int i;

	/* alocate memory data */
	system->item_list = malloc(sizeof(*system->item_list));
	
	if (system->item_list != NULL)
	{
		system->item_list->head = NULL;
		system->item_list->count = 0;
	}

	/* intiliase the cash register */
	for (i = 0; i < NUM_DENOMS; i++)
	{
		system->cash_register[i].denom = NO_CASH;
		system->cash_register[i].count = 0;
	}

	system->stock_file_name = NULL;
	system->coin_file_name = NULL;

    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
	struct ppd_node * temp;
	struct ppd_node * node;

	node = system->item_list->head;

	while (node != NULL)
	{
		temp = node;
		node = node->next;

		/* free current node */
		free(temp->data);
		free(temp);
	}
	free(system->item_list);
}
