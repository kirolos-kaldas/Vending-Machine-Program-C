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

#include "ppd_options.h"
#include "ppd_utility.h"

/**
* @file ppd_options.c this is where you need to implement the main
* options for your program. You may however have the actual work done
* in functions defined elsewhere.
* @note if there is an error you should handle it within the function
* and not simply return FALSE unless it is a fatal error for the
* task at hand. You want people to use your software, afterall, and
* badly behaving software doesn't get used.
**/

/* Checks if the new item id already exists */
BOOLEAN check_item_id(char * id, struct ppd_system * system)
{
	struct ppd_node * previousNode, *currentNode;
	int i, true = 0, false = 0;

	previousNode = NULL;
	currentNode = system->item_list->head;

	for (i = 0; i < system->item_list->count; i++)
	{
		/* Checks if the current item id is equal to the 
		new item id to send false */
		if (strcmp(id, currentNode->data->id) == 0)
		{
			false++;
		}
		else
		{
			true++;
		}

		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	if (true == system->item_list->count)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* Checks the dollars format */
int check_item_price(char * price)
{
	char * token;
	int no_dollars, i, test;
	char new_price[PRICELEN + 2];
	no_dollars = 2;

	memset(new_price, '\0', sizeof(new_price));
	strcpy(new_price, price);

	test = 0;

	/* Get total '.' in the price */
	for (i = 0; i < strlen(new_price); i++)
	{
		if (new_price[i] == '.')
		{
			test++;
		}
	}

	/* if there's no '.' then price is wrong */
	if (test == 0)
	{
		fprintf(stderr, "Error: The price is not valid.");
		return FALSE;
	}

	/* if '.' is the first char in the price then there was no 
	dollars inputed */
	if (new_price[0] != '.')
	{
		token = strtok(new_price, ".");

		if (token != NULL)
		{
			if (strlen(token) == 2)
			{
				/* Check if the dollars are digits or not */
				if (isdigit(token[0]) && isdigit(token[1]))
				{
					return TRUE;
				}
				else
				{
					fprintf(stderr, "Error: The price is not valid.");
					return FALSE;
				}
			}
			else
			{
				if (isdigit(token[0]))
				{
					return TRUE;
				}
				else
				{
					fprintf(stderr, "Error: The price is not valid.");
					return FALSE;
				}
			}
		}
		else
		{
			fprintf(stderr, "Error: The price is not valid.");
			return FALSE;
		}
	}
	else
	{
		return no_dollars;
	}
}

/* Check the format of the cents */
BOOLEAN check_cents(char * price)
{
	char * token;
	int test, i;
	char new_cents[PRICELEN + 2];

	memset(new_cents, '\0', sizeof(new_cents));
	strcpy(new_cents, price);

	test = 0;


	for (i = 0; i < strlen(new_cents); i++)
	{
		if (new_cents[i] == '.')
		{
			test++;
		}
	}


	/* if there's more '.' than one in the price then it is invalid */
	if (test > 1)
	{
		fprintf(stderr, "Error: Cents value is not valid.");
		return FALSE;
	}

	/* Get where the '.' char is*/
	for (i = 0; i < strlen(new_cents); i++)
	{
		if (new_cents[i] == '.')
		{
			break;
		}
	}

	/* if '.' is at zero then the first token is the cents value else
	the first is the dollars and the second is the cents*/
	if (i == 0)
	{
		token = strtok(new_cents, ".");
	}
	else
	{
		token = strtok(new_cents, ".");
		token = strtok(NULL, ".");
	}

	if (token != NULL)
	{
		if (strlen(token) == 2)
		{
			/* check if the cents value is digit */
			if (isdigit(token[0]) && isdigit(token[1]))
			{
				/* check if the cents are a multiple of five */
				if (token[1] == '0' || token[1] == '5')
				{
					return TRUE;
				}
				else
				{
					fprintf(stderr, "Error: The cents need to be a multiple"
						" of 5.");
					return FALSE;
				}
			}
			else
			{
				fprintf(stderr, "Error: Cents value is not valid.");
				return FALSE;
			}
		}
		else
		{
			fprintf(stderr, "Error: Cents value is not valid.");
			return FALSE;
		}
	}
	else
	{
		fprintf(stderr, "Error: Cents value is not valid.");
		return FALSE;
	}
}

/* Check the price format */
BOOLEAN check_price_format(char * price)
{
	int i;
	char input_price[PRICELEN + 2];

	memset(input_price, '\0', sizeof(input_price));
	strcpy(input_price, price);

	/* check if the price doent contain characters other than numbers */
	for (i = 0; i < strlen(input_price); i++)
	{
		if (!isdigit(input_price[i]))
		{
			fprintf(stderr, "Error: input was not a number. Please "
				"Try Again.\n");
			return FALSE;
		}
	}

	return TRUE;
}

/* Covert the token of cents to an int */
int covert_to_cents(char * price)
{
	char input_price[PRICELEN + 2];
	char * tokenEndPtr;
	int cents;

	memset(input_price, '\0', sizeof(input_price));
	strcpy(input_price, price);

	/* change to int */
	cents = (int)strtol(input_price, &tokenEndPtr, 10);

	return cents;
}

/* this function checks if the coin entered is valid or not */
BOOLEAN validating_price(int price, struct ppd_system * system)
{
	int i;

	if (price == TEN_DOLLARS_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TEN_DOLLARS)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else if (price == FIVE_DOLLARS_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == FIVE_DOLLARS)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else if (price == TWO_DOLLARS_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TWO_DOLLARS)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else if (price == ONE_DOLLAR_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == ONE_DOLLAR)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else if (price == FIFTY_CENTS_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == FIFTY_CENTS)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else if (price == TWENTY_CENTS_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TWENTY_CENTS)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else if (price == TEN_CENTS_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TEN_CENTS)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else if (price == FIVE_CENTS_DENOM)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == FIVE_CENTS)
			{
				/* increment the count for the coin */
				system->cash_register[i].count++;
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* check the amount of coins in the system */
int check_amount_coin(enum denomination denom, struct ppd_system * system,
	int coins, int * deduction)
{
	int i;
	int more_coins = 2;

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == denom)
		{
			/* Check if the coins in the system are zero or more 
			than coins needed to give back change */
			if (system->cash_register[i].count == 0 || 
				system->cash_register[i].count < coins)
			{
				if (system->cash_register[i].count < coins)
				{
					/* calculate the value of the deduction to be deducted to
					the needed amount of change */
					*deduction = coins - system->cash_register[i].count;
					return more_coins;
				}
				else
				{
					*deduction = 0;
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

/* give the change to the customer */
void get_change(double amount, struct ppd_system * system)
{
	int i, j, deduction;
	int num_ten_dollars, num_five_dollars, num_two_dollars, num_one_dollars,
		num_fifty_cents, num_twenty_cents, num_ten_cents, num_five_cents;
	/* get the price in cents */
	int change = (int)(amount * 100);
	int test;

	/* get the change of ten dollars */
	num_ten_dollars = change / TEN_DOLLARS_DENOM;
	
	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(TEN_DOLLARS, system, num_ten_dollars, &deduction);
	
	/* if the system doesnt have enough coins then make them equal 
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_ten_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % TEN_DOLLARS_DENOM;
	}
	else
	{
		num_ten_dollars -= deduction;
		change -= num_ten_dollars * TEN_DOLLARS_DENOM;
	}
	
	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_ten_dollars; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TEN_DOLLARS)
			{
				system->cash_register[i].count--;
				printf("$10 ");
			}
		}
	}

	/* get the change of five dollars */
	num_five_dollars = change / FIVE_DOLLARS_DENOM;

	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(FIVE_DOLLARS, system, num_five_dollars, &deduction);

	/* if the system doesnt have enough coins then make them equal
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_five_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % FIVE_DOLLARS_DENOM;
	}
	else
	{
		num_five_dollars -= deduction;
		change -= num_five_dollars * FIVE_DOLLARS_DENOM;
	}
	
	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_five_dollars; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == FIVE_DOLLARS)
			{
				system->cash_register[i].count--;
				printf("$5 ");
			}
		}
	}

	/* get the change of two dollars */
	num_two_dollars = change / TWO_DOLLARS_DENOM;

	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(TWO_DOLLARS, system, num_two_dollars, &deduction);

	/* if the system doesnt have enough coins then make them equal
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_two_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % TWO_DOLLARS_DENOM;
	}
	else
	{
		num_two_dollars -= deduction;
		change -= num_two_dollars * TWO_DOLLARS_DENOM;
	}

	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_two_dollars; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TWO_DOLLARS)
			{
				system->cash_register[i].count--;
				printf("$2 ");
			}
		}
	}

	/* get the change of one dollar */
	num_one_dollars = change / ONE_DOLLAR_DENOM;

	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(ONE_DOLLAR, system, num_one_dollars, &deduction);

	/* if the system doesnt have enough coins then make them equal
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_one_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % ONE_DOLLAR_DENOM;
	}
	else
	{
		num_one_dollars -= deduction;
		change -= num_one_dollars * ONE_DOLLAR_DENOM;
	}
	
	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_one_dollars; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == ONE_DOLLAR)
			{
				system->cash_register[i].count--;
				printf("$1 ");
			}
		}
	}
	
	/* get the change of fifty cents */
	num_fifty_cents = change / FIFTY_CENTS_DENOM;

	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(FIFTY_CENTS, system, num_fifty_cents, &deduction);

	/* if the system doesnt have enough coins then make them equal
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_fifty_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % FIFTY_CENTS_DENOM;
	}
	else
	{
		num_fifty_cents -= deduction;
		change -= num_fifty_cents * FIFTY_CENTS_DENOM;
	}

	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_fifty_cents; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == FIFTY_CENTS)
			{
				system->cash_register[i].count--;
				printf("50c ");
			}
		}
	}
	
	/* get the change of twenty cents */
	num_twenty_cents = change / TWENTY_CENTS_DENOM;

	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(TWENTY_CENTS, system, num_twenty_cents, &deduction);

	/* if the system doesnt have enough coins then make them equal
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_twenty_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % TWENTY_CENTS_DENOM;
	}
	else
	{
		num_twenty_cents -= deduction;
		change -= num_twenty_cents * TWENTY_CENTS_DENOM;
	}

	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_twenty_cents; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TWENTY_CENTS)
			{
				system->cash_register[i].count--;
				printf("20c ");
			}
		}
	}

	/* get the change of ten cents */
	num_ten_cents = change / TEN_CENTS_DENOM;

	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(TEN_CENTS, system, num_ten_cents, &deduction);

	/* if the system doesnt have enough coins then make them equal
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_ten_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % TEN_CENTS_DENOM;
	}
	else
	{
		num_ten_cents -= deduction;
		change -= num_ten_cents * TEN_CENTS_DENOM;
	}

	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_ten_cents; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == TEN_CENTS)
			{
				system->cash_register[i].count--;
				printf("10c ");
			}
		}
	}
	
	/* get the change of five cents */
	num_five_cents = change / FIVE_CENTS_DENOM;

	/* check the amount of the coins in the system before getting change */
	test = check_amount_coin(FIVE_CENTS, system, num_five_cents, &deduction);

	/* if the system doesnt have enough coins then make them equal
	to zero, else if true, get the rest of the change needed, else, deducte from
	number of coins and get the rest of the change needed */
	if (test == FALSE)
	{
		num_five_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % FIVE_CENTS_DENOM;
	}
	else
	{
		num_five_cents -= deduction;
		change -= num_five_cents * FIVE_CENTS_DENOM;
	}
	
	/* Decrease the coins in the system and print the coin for every
	number of coins needed to give back change */
	for (j = 0; j < num_five_cents; j++)
	{
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (system->cash_register[i].denom == FIVE_CENTS)
			{
				system->cash_register[i].count--;
				printf("5c ");
			}
		}
	}
}

/* check if it is possible to give back change */
BOOLEAN check_system_change(double amount, struct ppd_system * system)
{
	int total_change, deduction = 0;
	int num_ten_dollars, num_five_dollars, num_two_dollars, num_one_dollars,
		num_fifty_cents, num_twenty_cents, num_ten_cents, num_five_cents;
	int change = (int)(amount * 100);
	int test;

	num_ten_dollars = change / TEN_DOLLARS_DENOM;
	test = check_amount_coin(TEN_DOLLARS, system, num_ten_dollars, &deduction);
	if (test == FALSE)
	{
		num_ten_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % TEN_DOLLARS_DENOM;
	}
	else
	{
		num_ten_dollars -= deduction;
		change -= num_ten_dollars * TEN_DOLLARS_DENOM;
	}

	
	num_five_dollars = change / FIVE_DOLLARS_DENOM;
	test = check_amount_coin(FIVE_DOLLARS, system, num_five_dollars, &deduction);
	if (test == FALSE)
	{
		num_five_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % FIVE_DOLLARS_DENOM;
	}
	else
	{
		num_five_dollars -= deduction;
		change -= num_five_dollars * FIVE_DOLLARS_DENOM;
	}


	num_two_dollars = change / TWO_DOLLARS_DENOM;
	test = check_amount_coin(TWO_DOLLARS, system, num_two_dollars, &deduction);
	if (test == FALSE)
	{
		num_two_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % TWO_DOLLARS_DENOM;
	}
	else
	{
		num_two_dollars -= deduction;
		change -= num_two_dollars * TWO_DOLLARS_DENOM;
	}


	num_one_dollars = change / ONE_DOLLAR_DENOM;
	test = check_amount_coin(ONE_DOLLAR, system, num_one_dollars, &deduction);
	if (test == FALSE)
	{
		num_one_dollars = 0;
	}
	else if (test == TRUE)
	{
		change = change % ONE_DOLLAR_DENOM;
	}
	else
	{
		num_one_dollars -= deduction;
		change -= num_one_dollars * ONE_DOLLAR_DENOM;
	}


	num_fifty_cents = change / FIFTY_CENTS_DENOM;
	test = check_amount_coin(FIFTY_CENTS, system, num_fifty_cents, &deduction);
	if (test == FALSE)
	{
		num_fifty_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % FIFTY_CENTS_DENOM;
	}
	else
	{
		num_fifty_cents -= deduction;
		change -= num_fifty_cents * FIFTY_CENTS_DENOM;
	}


	num_twenty_cents = change / TWENTY_CENTS_DENOM;
	test = check_amount_coin(TWENTY_CENTS, system, num_twenty_cents, &deduction);
	if (test == FALSE)
	{
		num_twenty_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % TWENTY_CENTS_DENOM;
	}
	else
	{
		num_twenty_cents -= deduction;
		change -= num_twenty_cents * TWENTY_CENTS_DENOM;
	}


	num_ten_cents = change / TEN_CENTS_DENOM;
	test = check_amount_coin(TEN_CENTS, system, num_ten_cents, &deduction);
	if (test == FALSE)
	{
		num_ten_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % TEN_CENTS_DENOM;
	}
	else
	{
		num_ten_cents -= deduction;
		change -= num_ten_cents * TEN_CENTS_DENOM;
	}


	num_five_cents = change / FIVE_CENTS_DENOM;
	test = check_amount_coin(FIVE_CENTS, system, num_five_cents, &deduction);
	if (test == FALSE)
	{
		num_five_cents = 0;
	}
	else if (test == TRUE)
	{
		change = change % FIVE_CENTS_DENOM;
	}
	else
	{
		num_five_cents -= deduction;
		change -= num_five_cents * FIVE_CENTS_DENOM;
	}


	total_change = num_ten_dollars + num_five_dollars + num_two_dollars + 
		num_one_dollars + num_fifty_cents + num_twenty_cents + 
		num_ten_cents + num_five_cents;

	if (total_change > 20)
	{
		fprintf(stderr, "Error: we were not able to add another coin to the "
			"change stack.\n");
		return FALSE;
	}


	if (total_change == 0 || change != 0)
	{
		fprintf(stderr, "Error: We don't have sufficient currency to give you "
			"change. Sorry about that.\n");
		return FALSE;
	}

	return TRUE;
}

/**
* Print all the items stored in the system
**/
BOOLEAN display_items(struct ppd_system * system)
{
	struct ppd_node * previousNode, * currentNode;

	printf("\nItems Menu\n");
	printf("----------\n");
	printf("ID\t|Name\t\t\t\t\t |Available |Price\n");
	printf("----------------------------------------------------------------"
		"---\n");

	previousNode = NULL;
	currentNode = system->item_list->head;

	/* loop through every node and print the item */
	while (currentNode != NULL)
	{
		printf("%s\t|%-40s|%d\t    |$ %d.%02d\n", currentNode->data->id,
			currentNode->data->name, currentNode->data->on_hand,
			currentNode->data->price.dollars, currentNode->data->price.cents);

		previousNode = currentNode;
		currentNode = currentNode->next;
	}
	printf("\n");

	return TRUE;
}

/**
* Purchase the item desired
**/
BOOLEAN purchase_item(struct ppd_system * system)
{
	struct ppd_node * previousNode, * currentNode;
	char id[IDLEN + 2];
	char price[PRICELEN + 2];
	BOOLEAN test;
	double dollars, item_dollars, amount_left, taken_money = 0;
	int j, cents, item_cents;

	memset(id, '\0', sizeof(id));

	printf("\nPurchase Item");
	printf("\n-------------");

	do
	{
		j = 0;
		previousNode = NULL;
		currentNode = system->item_list->head;

		do
		{
			printf("\nPlease enter the ID of the item you wish to purchase: ");

			/* get item id input */
			if (fgets(id, IDLEN + 2, stdin) == NULL)
			{
				return FALSE;
			}

			if (strcmp(id, "\n") == 0)
			{
				return FALSE;
			}

			if (id[strlen(id) - 1] != '\n')
			{
				fprintf(stderr, "Error: The id entered is too long and doesn't "
					"exist. Please Try Again.");
				read_rest_of_line();
				test = FALSE;
			}
			else
			{
				id[strlen(id) - 1] = '\0';
				test = TRUE;
			}
		} while (test == FALSE);

		if (system->item_list->count == 0)
		{
			test = FALSE;
			fprintf(stderr, "Error: you did not enter a valid id. Please Try "
				"Again.");
		}
		else
		{
			while (currentNode != NULL)
			{
				if (strcmp(id, currentNode->data->id) == 0)
				{
					/* check if the system has the item to be purchased on 
					hand, else re-ask for new input */
					if (currentNode->data->on_hand != 0)
					{
						test = TRUE;
					}
					else
					{
						fprintf(stderr, "Error: there is none of %s on hand. "
							"Please choose another item.",
							currentNode->data->name);
						test = FALSE;
					}
					break;
				}
				else
				{
					j++;
					/* check if the item exists */
					if (j == system->item_list->count)
					{
						test = FALSE;
						fprintf(stderr, "Error: you did not enter a valid id."
							" Please Try Again.");
					}
				}

				previousNode = currentNode;
				currentNode = currentNode->next;
			}
		}

	} while (test == FALSE);

	printf("You have selected \"%s - %s\". This will cost you $%d.%02d\n", 
		currentNode->data->name, currentNode->data->desc, 
		currentNode->data->price.dollars, currentNode->data->price.cents);

	printf("Please hand over the money - type in the value of each note/coin"
		" in cents.\n");
	printf("Press enter or ctrl+d on a new line to cancel this purchase:\n");

	/* get the total item price in cents */
	item_cents = (currentNode->data->price.dollars * 100) +
		currentNode->data->price.cents;

	/* get the total item price in dollars */
	item_dollars = (double)item_cents / (double)100;

	/* the amount to be paid is equal to the price of the item */
	amount_left = item_dollars;

	do
	{
		do
		{
			printf("You still need to give us $%.02f: ", amount_left);

			if (fgets(price, PRICELEN + 2, stdin) == NULL)
			{
				printf("\nChange of mind - here is your change:\n");
				/* give back the money inputed by the user */
				get_change(taken_money, system);
				printf("\nPlease come again soon.\n");
				return TRUE;
			}

			if (strcmp(price, "\n") == 0)
			{
				printf("Change of mind - here is your change:\n");
				/* give back the money inputed by the user */
				get_change(taken_money, system);
				printf("\nPlease come again soon.\n");
				return TRUE;
			}

			if (price[strlen(price) - 1] != '\n')
			{
				fprintf(stderr, "Error: The line entered is too long. Please "
					"Try Again.\n");
				read_rest_of_line();
				test = FALSE;
			}
			else
			{
				price[strlen(price) - 1] = '\0';
				test = TRUE;
			}
		} while (test == FALSE);

		/* check the format of the price */
		test = check_price_format(price);

		if (test != FALSE)
		{
			/* change input price to cents */
			cents = covert_to_cents(price);
			dollars = (double)cents / (double)100;

			/* add the dollars inputed to the money taken in the system */
			taken_money += dollars;
			
			/* check if the price is valid */
			if (validating_price(cents, system) == FALSE)
			{
				printf("Error: $%.02f is not a valid denomination of money."
					" Please try again.\n", dollars);
				test = FALSE;
			}
			else
			{
				/* Get the rest of the price to be paid */
				amount_left = amount_left - dollars;

				/* checks if there is more to be paid to go through the loop 
				again */
				if (amount_left > 0)
				{
					test = FALSE;
				}
				else if (amount_left == 0)
				{
					test = TRUE;
					printf("Here is your %s.\n", currentNode->data->name);
				}
				else
				{
					/* change the negative value of amount left
					to positive */
					amount_left = 0 - amount_left;

					/* check if it is possible to give back change */
					if (check_system_change(amount_left, system) != FALSE)
					{
						printf("Here is your %s. And your change of $ %.02f: ",
							currentNode->data->name, amount_left);
						get_change(amount_left, system);
						test = TRUE;
					}
					else
					{
						printf("Unfortunately, we could not give you change for "
							"your purchase. Instead, here is your refund: ");
						/* gives back the money given by the user */
						get_change(taken_money, system);
						return FALSE;
					}
				}
			}
		}
	} while (test == FALSE);
	
	/* decrease the amount of the item in the system */
	currentNode->data->on_hand--;

	printf("\nPlease come again soon.\n");

	return TRUE;
}

/**
* Save the data in the system to the files that were passed in
**/
BOOLEAN save_system(struct ppd_system * system)
{
	FILE * stock_file, * coins_file;
	struct ppd_node * previousNode, *currentNode;
	int i;

	/* open file */
	stock_file = fopen(system->stock_file_name, "w");

	if (stock_file == NULL)
	{
		fprintf(stderr, "\nfailed to open %s: No such file or directory\n", system->stock_file_name);
		return FALSE;
	}

	previousNode = NULL;
	currentNode = system->item_list->head;

	/* Loop through the nodes in the system and saves the data in 
	the them */
	while (currentNode != NULL)
	{
		fprintf(stock_file, "%s|", currentNode->data->id);
		fprintf(stock_file, "%s|", currentNode->data->name);
		fprintf(stock_file, "%s|", currentNode->data->desc);
		fprintf(stock_file, "%d.", currentNode->data->price.dollars);
		fprintf(stock_file, "%02d|", currentNode->data->price.cents);
		fprintf(stock_file, "%d\n", currentNode->data->on_hand);

		previousNode = currentNode;
		currentNode = currentNode->next;
	}
	fclose(stock_file);

	/* open file */
	coins_file = fopen(system->coin_file_name, "w");

	if (stock_file == NULL)
	{
		fprintf(stderr, "\nfailed to open %s: No such file or directory\n", system->coin_file_name);
		return FALSE;
	}

	/* loops through the array of the cash register and saves
	the data */
	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == TEN_DOLLARS)
		{
			fprintf(coins_file, "%d,", TEN_DOLLARS_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
		else if (system->cash_register[i].denom == FIVE_DOLLARS)
		{
			fprintf(coins_file, "%d,", FIVE_DOLLARS_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
		else if (system->cash_register[i].denom == TWO_DOLLARS)
		{
			fprintf(coins_file, "%d,", TWO_DOLLARS_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
		else if (system->cash_register[i].denom == ONE_DOLLAR)
		{
			fprintf(coins_file, "%d,", ONE_DOLLAR_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
		else if (system->cash_register[i].denom == FIFTY_CENTS)
		{
			fprintf(coins_file, "%d,", FIFTY_CENTS_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
		else if (system->cash_register[i].denom == TWENTY_CENTS)
		{
			fprintf(coins_file, "%d,", TWENTY_CENTS_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
		else if (system->cash_register[i].denom == TEN_CENTS)
		{
			fprintf(coins_file, "%d,", TEN_CENTS_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
		else if (system->cash_register[i].denom == FIVE_CENTS)
		{
			fprintf(coins_file, "%d,", FIVE_CENTS_DENOM);
			fprintf(coins_file, "%d\n", system->cash_register[i].count);
		}
	}
	fclose(coins_file);

	return TRUE;
}

/**
* Add a new item to the system
**/
BOOLEAN add_item(struct ppd_system * system)
{
	struct ppd_node * previousNode, *currentNode;
	struct ppd_node * newNode;
	char new_id[IDLEN + 1];
	char new_name[NAMELEN + 1];
	char new_desc[DESCLEN + 1];
	char new_price[PRICELEN + 2];
	char * tokenEndPtr;
	char * token;
	int new_item_number;
	BOOLEAN test;
	int no_dollars = 2;

	new_item_number = 1;

	newNode = malloc(sizeof(*newNode));
	if (newNode == NULL)
	{
		return FALSE;
	}

	newNode->data = malloc(sizeof(*newNode->data));
	if (newNode->data == NULL)
	{
		free(newNode);
		return FALSE;
	}

	newNode->next = NULL;

	previousNode = NULL;
	currentNode = system->item_list->head;

	do
	{
		/* creates the id in the right format */
		sprintf(new_id, "I%04d", new_item_number);

		/* checks if the id is already in the system */
		if (check_item_id(new_id, system) == FALSE)
		{
			/*  increments the id number and then 
			rechecks */
			new_item_number++;
			test = FALSE;
		}
		else
		{
			test = TRUE;
		}
	} while (test == FALSE);

	printf("\nThe ID of the new stock will be: %s", new_id);
	strcpy(newNode->data->id, new_id);

	do
	{
		printf("\nEnter the item name: ");
		if (fgets(new_name, NAMELEN + 1, stdin) == NULL)
		{
			printf("\nCancelling \"Add Item\" at user's request.");
			return FALSE;
		}

		if (strcmp(new_name, "\n") == 0)
		{
			printf("Cancelling \"Add Item\" at user's request.");
			return FALSE;
		}

		if (new_name[strlen(new_name) - 1] != '\n')
		{
			fprintf(stderr, "Error: The name entered is too long. "
				"Please Try Again.");
			read_rest_of_line();
			test = FALSE;
		}
		else
		{
			new_name[strlen(new_name) - 1] = '\0';
			test = TRUE;
		}
	} while (test == FALSE);
	strcpy(newNode->data->name, new_name);

	do
	{
		printf("Enter the item description: ");
		if (fgets(new_desc, DESCLEN + 1, stdin) == NULL)
		{
			printf("\nCancelling \"Add Item\" at user's request.");
			return FALSE;
		}

		if (strcmp(new_desc, "\n") == 0)
		{
			printf("Cancelling \"Add Item\" at user's request.");
			return FALSE;
		}

		if (new_desc[strlen(new_desc) - 1] != '\n')
		{
			fprintf(stderr, "Error: The description entered is too "
				"long. Please Try Again.");
			read_rest_of_line();
			test = FALSE;
		}
		else
		{
			new_desc[strlen(new_desc) - 1] = '\0';
			test = TRUE;
		}
	} while (test == FALSE);
	strcpy(newNode->data->desc, new_desc);

	do
	{
		do
		{
			printf("Enter the price for the item: ");
			if (fgets(new_price, PRICELEN + 2, stdin) == NULL)
			{
				printf("\nCancelling \"Add Item\" at user's request.");
				return FALSE;
			}

			if (strcmp(new_price, "\n") == 0)
			{
				printf("Cancelling \"Add Item\" at user's request.");
				return FALSE;
			}

			if (new_price[strlen(new_price) - 1] != '\n')
			{
				fprintf(stderr, "Error: The line entered is too long."
					" Please Try Again.\n");
				read_rest_of_line();
				test = FALSE;
			}
			else
			{
				new_price[strlen(new_price) - 1] = '\0';
				test = TRUE;
			}
		} while (test == FALSE);

		/* Checks the dollars of the price */
		test = check_item_price(new_price);
		
		/* if the price is valid, else if there is
		no dollars inuted ie ".50" */
		if (test == TRUE)
		{
			/* Checks the cents of the price */
			test = check_cents(new_price);
			
			if (test != FALSE)
			{
				token = strtok(new_price, ".");
				newNode->data->price.dollars = strtol(token, 
					&tokenEndPtr, 10);

				token = strtok(NULL, ".");
				newNode->data->price.cents = strtol(token, 
					&tokenEndPtr, 10);
				token = strtok(NULL, ".");
			}
			else
			{
				printf(" Please Try Again.\n");
			}
		}
		else if (test == no_dollars)
		{
			/* Checks the cents of the price */
			test = check_cents(new_price);

			if (test != FALSE)
			{
				newNode->data->price.dollars = 0;

				token = strtok(new_price, ".");
				newNode->data->price.cents = strtol(token, 
					&tokenEndPtr, 10);
				token = strtok(NULL, ".");
			}
			else
			{
				printf(" Please Try Again.\n");
			}
		}
		else
		{
			printf(" Please Try Again.\n");
		}
	} while (test == FALSE);
	
	newNode->data->on_hand = DEFAULT_STOCK_LEVEL;

	printf("This item \"%s - %s\" has now been added to the menu.\n",
		newNode->data->name, newNode->data->desc);

	while (currentNode != NULL)
	{
		/* add the new item in the right order by name */
		if (strcmp(newNode->data->name, currentNode->data->name) < 0)
		{
			break;
		}

		/* loop through each node */
		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	/* checks if the head is empty to store at the head
	or if it is inserting at the head but the head has a node */
	if (system->item_list->head == NULL)
	{
		system->item_list->head = newNode;
	}
	else if (previousNode == NULL)
	{
		newNode->next = system->item_list->head;
		system->item_list->head = newNode;
	}
	else
	{
		previousNode->next = newNode;
		newNode->next = currentNode;
	}
	system->item_list->count++;

	return TRUE;
}

/**
* Removes the unwanted item
**/
BOOLEAN remove_item(struct ppd_system * system)
{
	struct ppd_node * previousNode, *currentNode;
	char id[IDLEN + 2];
	BOOLEAN test;
	int j = 0;

	memset(id, '\0', sizeof(id));

	do
	{
		previousNode = NULL;
		currentNode = system->item_list->head;

		do
		{
			printf("\nEnter the item id of the item to remove from the menu: ");
			if (fgets(id, IDLEN + 2, stdin) == NULL)
			{
				printf("\nCancelling \"Remove Item\" at user's request.");
				return FALSE;
			}

			if (strcmp(id, "\n") == 0)
			{
				printf("Cancelling \"Remove Item\" at user's request.");
				return FALSE;
			}

			if (id[strlen(id) - 1] != '\n')
			{
				fprintf(stderr, "Error: The id entered is too long and doesn't"
					" exist. Please Try Again.");
				read_rest_of_line();
				test = FALSE;
			}
			else
			{
				id[strlen(id) - 1] = '\0';
				test = TRUE;
			}
		} while (test == FALSE);

		/* checks if the system is empty before removing */
		if (system->item_list->count == 0)
		{
			fprintf(stderr, "Error: The desired ID was not found.\n");
			fprintf(stderr, "Error: %s was not an id in the stock list.", id);
			return FALSE;
		}

		while (currentNode != NULL)
		{
			/* if the current node id is equal to the id inputed then break
			to delete that node */
			if (strcmp(id, currentNode->data->id) == 0)
			{
				test = TRUE;
				break;
			}
			else
			{
				j++;
				/* checks if the id inputed doesnt exist */
				if (j == system->item_list->count)
				{
					fprintf(stderr, "Error: The desired ID was not found.\n");
					fprintf(stderr, "Error: %s was not an id in the stock list.",
						id);
					return FALSE;
				}
			}

			previousNode = currentNode;
			currentNode = currentNode->next;
		}
	} while (test == FALSE);
	
	if (previousNode == NULL)
	{
		system->item_list->head = currentNode->next;
	}
	else
	{
		previousNode->next = currentNode->next;
	}
	/* decrease the amount of items in the system */
	system->item_list->count--;

	printf("\"%s - %s - %s\" has been removed from the system.\n", 
		currentNode->data->id,
		currentNode->data->name, currentNode->data->desc);

	/* free the node that was removed from the system */
	free(currentNode->data);
	free(currentNode);

	return TRUE;
}

/**
* Reset the stock to 20
**/
BOOLEAN reset_stock(struct ppd_system * system)
{
	struct ppd_node * previousNode, *currentNode;

	previousNode = NULL;
	currentNode = system->item_list->head;

	/* loop through every node */
	while (currentNode != NULL)
	{
		currentNode->data->on_hand = DEFAULT_STOCK_LEVEL;

		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	printf("\nAll stock has been reset to the default level of %d\n",
		DEFAULT_STOCK_LEVEL);

	return TRUE;
}

/**
* reset the coins' count to 20
**/
BOOLEAN reset_coins(struct ppd_system * system)
{
	int i;

	/* loop through every coin in the array */
	for (i = 0; i < NUM_DENOMS; i++)
	{
		system->cash_register[i].count = DEFAULT_COIN_COUNT;
	}

	printf("\nAll coins have been reset to the default level of %d\n", 
		DEFAULT_COIN_COUNT);

	return TRUE;
}

/**
* Display all coins
**/
BOOLEAN display_coins(struct ppd_system * system)
{
	int i;

	printf("\nCoins Summary\n");
	printf("-------------\n");
	printf("%-16s|%10s\n", "Denomination", "Count");
	printf("---------------------------\n");

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == FIVE_CENTS)
		{
			printf("%-16s|%10d\n", "5 Cents",
				system->cash_register[i].count);
		}
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == TEN_CENTS)
		{
			printf("%-16s|%10d\n", "10 Cents",
				system->cash_register[i].count);
		}
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == TWENTY_CENTS)
		{
			printf("%-16s|%10d\n", "20 Cents",
				system->cash_register[i].count);
		}
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == FIFTY_CENTS)
		{
			printf("%-16s|%10d\n", "50 Cents",
				system->cash_register[i].count);
		}
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == ONE_DOLLAR)
		{
			printf("%-16s|%10d\n", "1 Dollar",
				system->cash_register[i].count);
		}
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == TWO_DOLLARS)
		{
			printf("%-16s|%10d\n", "2 Dollar",
				system->cash_register[i].count);
		}
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == FIVE_DOLLARS)
		{
			printf("%-16s|%10d\n", "5 Dollar",
				system->cash_register[i].count);
		}
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == TEN_DOLLARS)
		{
			printf("%-16s|%10d\n", "10 Dollar", 
				system->cash_register[i].count);
		}
	}

	return TRUE;
}
