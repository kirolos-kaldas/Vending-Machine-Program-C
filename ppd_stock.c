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
#include "ppd_stock.h"
#include "ppd_main.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/* checks the id's format */
BOOLEAN check_id(char * check_id)
{
	int i;
	char id[IDLEN + 1];

	if (strlen(check_id) > IDLEN)
	{
		return FALSE;
	}

	memset(id, '\0', sizeof(id));
	strcpy(id, check_id);

	if (strlen(id) == IDLEN)
	{
		if (id[0] == 'I')
		{
			for (i = 1; i < IDLEN; i++)
			{
				if (!isdigit(id[i]))
				{
					return FALSE;
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

/* checks the dollars length */
BOOLEAN check_dollars_length(char * price)
{
	char dollar[PRICELEN];

	if (strlen(price) > PRICELEN)
	{
		return FALSE;
	}

	memset(dollar, '\0', sizeof(dollar));
	strcpy(dollar, price);

	if (strlen(dollar) > 2 && strlen(dollar) < 0)
	{
		return FALSE;
	}

	return TRUE;
}

/* checks the length of the cents */
BOOLEAN check_cents_length(char * price)
{
	char cents[PRICELEN];

	if (strlen(price) > PRICELEN)
	{
		return FALSE;
	}

	memset(cents, '\0', sizeof(cents));
	strcpy(cents, price);

	if (strlen(cents) > 2 && strlen(cents) < 0)
	{
		return FALSE;
	}

	return TRUE;
}

/* checks the range of the on hand */
BOOLEAN check_onhand(char * quantity)
{
	int i;
	char onhand[QUANTITYLEN];

	if (strlen(quantity) > QUANTITYLEN)
	{
		fprintf(stderr, "\nError in stock file: the number for stock "
			"on hand was outside the valid range.\n");
		return FALSE;
	}

	memset(onhand, '\0', sizeof(onhand));
	strcpy(onhand, quantity);

	for (i = 0; i < strlen(onhand); i++)
	{
		if (!isdigit(onhand[i]))
		{
			fprintf(stderr, "\nError loading stock file: the number on "
				"hand was not a valid integer.\n");
			return FALSE;
		}
	}

	return TRUE;
}



 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/
BOOLEAN load_stock(const char * stock_name, struct ppd_system * system)
{
	FILE * stock_file;
	struct ppd_node * previousNode, * currentNode;
	struct ppd_node * newNode;
	char line[LINE_LEN + 1];
	char * token, *price_token;
	char * tokenEndPtr;
	int no_dollars = 2;

	stock_file = fopen(stock_name, "r");

	if (stock_file == NULL)
	{
		fprintf(stderr, "\nfailed to open %s: No such file or directory\n", 
			stock_name);
		return FALSE;
	}

	while (fgets(line, sizeof(line), stock_file) != NULL)
	{
		if (line[0] != '\n')
		{
			if (line[strlen(line) - 1] != '\n')
			{
				fprintf(stderr, "\nError: line too long in loading stock file.\n");
				fclose(stock_file);
				return FALSE;
			}
			else
			{
				/* Line length was ok. Remove the '\n' character. */
				line[strlen(line) - 1] = '\0';
			}

			/* create a node to hold the tokenized data */
			newNode = malloc(sizeof(*newNode));
			if (newNode == NULL)
			{
				fclose(stock_file);
				return FALSE;
			}

			newNode->data = malloc(sizeof(*newNode->data));
			if (newNode->data == NULL)
			{
				free(newNode);
				fclose(stock_file);
				return FALSE;
			}

			newNode->next = NULL;

			previousNode = NULL;
			currentNode = system->item_list->head;

			token = strtok(line, "|");
			while (token != NULL)
			{
				if (check_id(token) == FALSE)
				{
					fprintf(stderr, "\nError loading stock file: the id token "
						"is not valid.\n");
					fclose(stock_file);
					return FALSE;
				}
				strcpy(newNode->data->id, token);

				token = strtok(NULL, "|");
				if (strlen(token) > NAMELEN)
				{
					fprintf(stderr, "\nError loading stock file: The name token "
						"is not valid.\n");
					fclose(stock_file);
					return FALSE;
				}
				strcpy(newNode->data->name, token);

				token = strtok(NULL, "|");
				if (strlen(token) > DESCLEN)
				{
					fprintf(stderr, "\nError loading stock file: The description "
						"token is not valid.\n");
					fclose(stock_file);
					return FALSE;
				}
				strcpy(newNode->data->desc, token);

				token = strtok(NULL, "|");
				price_token = token;
				if (strlen(price_token) > PRICELEN)
				{
					fprintf(stderr, "\nError in coin file: the price is invalid.\n");
					fclose(stock_file);
					return FALSE;
				}

				token = strtok(NULL, "|");
				if (token != NULL)
				{
					if (check_onhand(token) == FALSE)
					{
						fclose(stock_file);
						return FALSE;
					}
				}
				else
				{
					fprintf(stderr, "\nError in stock file: invalid number of"
						" tokens in line.\n");
					fclose(stock_file);
					return FALSE;
				}
				newNode->data->on_hand = strtol(token, &tokenEndPtr, 10);

				/* validates price */
				if (check_item_price(price_token) == FALSE)
				{
					fclose(stock_file);
					return FALSE;
				}
				else if (check_item_price(price_token) == no_dollars)
				{
					newNode->data->price.dollars = 0;

					/* validates cents */
					if (check_cents(price_token) == FALSE)
					{
						fclose(stock_file);
						return FALSE;
					}

					price_token = strtok(price_token, ".");
				}
				else
				{
					/* validates cents */
					if (check_cents(price_token) == FALSE)
					{
						fclose(stock_file);
						return FALSE;
					}

					price_token = strtok(price_token, ".");
					if (strlen(price_token) > PRICELEN)
					{
						fprintf(stderr, "\nError in coin file: the dollars "
							"are out of range.\n");
						fclose(stock_file);
						return FALSE;
					}
					if (check_cents_length(price_token) == FALSE)
					{
						fprintf(stderr, "\nError: the dollars are out of range.\n");
						fclose(stock_file);
						return FALSE;
					}
					newNode->data->price.dollars = strtol(price_token, &tokenEndPtr, 10);

					price_token = strtok(NULL, ".");
				}

				if (strlen(price_token) > PRICELEN)
				{
					fprintf(stderr, "\nError in coin file: the cents are out of range.\n");
					fclose(stock_file);
					return FALSE;
				}
				if (check_dollars_length(price_token) == FALSE)
				{
					fprintf(stderr, "\nError: The cents are out of range.\n");
					fclose(stock_file);
					return FALSE;
				}
				newNode->data->price.cents = strtol(price_token, &tokenEndPtr, 10);

				price_token = strtok(NULL, ".");

				token = strtok(NULL, "|");
				if (token != NULL)
				{
					fprintf(stderr, "\nError in stock file: invalid number of tokens.\n");
					fclose(stock_file);
					return FALSE;
				}

				while (currentNode != NULL)
				{
					if (strcmp(currentNode->data->name, newNode->data->name) > 0)
					{
						break;
					}

					previousNode = currentNode;
					currentNode = currentNode->next;
				}

				if (system->item_list->head == NULL)
				{
					system->item_list->head = newNode;
				}
				else if (strcmp(system->item_list->head->data->name, newNode->data->name) > 0)
				{
					system->item_list->head = newNode;
					system->item_list->head->next = currentNode;
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
			}
		}
	}

	fclose(stock_file);

	return TRUE;
}