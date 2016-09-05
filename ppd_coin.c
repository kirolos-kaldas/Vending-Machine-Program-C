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
#include "ppd_coin.h"
#include "ppd_main.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/* checks the line of of the coins and the counts */
BOOLEAN check_line(char * new_line)
{
	int i, test;
	char line[LINE_LEN + 1];

	memset(line, '\0', sizeof(line));
	strcpy(line, new_line);

	test = 0;

	if (strlen(line) == 1)
	{
		if (line[0] == ',')
		{
			return FALSE;
		}
	}

	for (i = 0; i < strlen(line); i++)
	{
		if (line[i] == ',')
		{
			test++;
		}
	}

	if (test > 1)
	{
		return FALSE;
	}
	
	return TRUE;
}

/* checks the count's format, length and range */
BOOLEAN check_count(char * token)
{
	int i, test = 0;
	char count[PRICELEN];
	char * tokenEndPtr;

	if (strlen(token) > PRICELEN)
	{
		fprintf(stderr, "\nError in coin file: The denomination count is too long.\n");
		return FALSE;
	}

	memset(count, '\0', sizeof(count));
	strcpy(count, token);

	for (i = 0; i < strlen(count); i++)
	{
		if (!isdigit(count[i]))
		{
			if (count[i] == '-' && test != 0)
			{
				fprintf(stderr, "\nError in coin file: non-numeric data encountered.\n");
				return FALSE;
			}
			else
			{
				test++;
			}
		}
	}

	if (strtol(count, &tokenEndPtr, 10) > 500 || strtol(count, &tokenEndPtr, 10) < 0)
	{
		fprintf(stderr, "\nError in coin file: %s is not a valid denomination count.\n", count);
		return FALSE;
	}

	return TRUE;
}

/* checks the coins' length and format */
BOOLEAN check_coin(char * token)
{
	int i;
	char coin[PRICELEN];

	if (strlen(token) > PRICELEN)
	{
		fprintf(stderr, "\nError in coin file: The coin denomination is too long.\n");
		return FALSE;
	}

	memset(coin, '\0', sizeof(coin));
	strcpy(coin, token);

	for (i = 0; i < strlen(coin); i++)
	{
		if (!isdigit(coin[i]))
		{
			fprintf(stderr, "\nError in coin file: non-numeric data encountered when trying to extract a coin.\n");
			return FALSE;
		}
	}

	return TRUE;
}

/* checks if the coin has been already entered in the system */
BOOLEAN check_coin_existence(enum denomination denom, struct ppd_system * system)
{
	int i;

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (system->cash_register[i].denom == denom)
		{
			fprintf(stderr, "\nError in coin file: the coin %d is already in the system.\n", denom);
			return FALSE;
		}
	}

	return TRUE;
}

 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/
BOOLEAN load_coins(const char * coins_name, struct ppd_system * system)
{
	FILE * coins_file;
	char line[PRICELEN + PRICELEN + 1];
	char * token;
	char * tokenEndPtr;
	int i;

	coins_file = fopen(coins_name, "r");

	if (coins_file == NULL)
	{
		fprintf(stderr, "\nfailed to open %s: No such file or directory\n", coins_name);
		fclose(coins_file);
		return FALSE;
	}

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (fgets(line, sizeof(line), coins_file) != NULL)
		{
			if (line[0] != '\n')
			{
				if (line[strlen(line) - 1] != '\n')
				{
					fprintf(stderr, "\nError: line too long in loading coin file.\n");
					fclose(coins_file);
					return FALSE;
				}
				else
				{
					/* Line length was ok. Remove the '\n' character. */
					line[strlen(line) - 1] = '\0';
				}

				if (check_line(line) == FALSE)
				{
					fprintf(stderr, "\nError: Invalid data in coin file.\n");
					fclose(coins_file);
					return FALSE;
				}

				token = strtok(line, COIN_DELIM);
				if (token)
				if (check_coin(token) == FALSE)
				{
					fclose(coins_file);
					return FALSE;
				}
				while (token != NULL)
				{
					if (strtol(token, &tokenEndPtr, 10) == TEN_DOLLARS_DENOM)
					{
						if (check_coin_existence(TEN_DOLLARS, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = TEN_DOLLARS;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else if (strtol(token, &tokenEndPtr, 10) == FIVE_DOLLARS_DENOM)
					{
						if (check_coin_existence(FIVE_DOLLARS, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = FIVE_DOLLARS;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else if (strtol(token, &tokenEndPtr, 10) == TWO_DOLLARS_DENOM)
					{
						if (check_coin_existence(TWO_DOLLARS, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = TWO_DOLLARS;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else if (strtol(token, &tokenEndPtr, 10) == ONE_DOLLAR_DENOM)
					{
						if (check_coin_existence(ONE_DOLLAR, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = ONE_DOLLAR;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else if (strtol(token, &tokenEndPtr, 10) == FIFTY_CENTS_DENOM)
					{
						if (check_coin_existence(FIFTY_CENTS, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = FIFTY_CENTS;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else if (strtol(token, &tokenEndPtr, 10) == TWENTY_CENTS_DENOM)
					{
						if (check_coin_existence(TWENTY_CENTS, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = TWENTY_CENTS;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else if (strtol(token, &tokenEndPtr, 10) == TEN_CENTS_DENOM)
					{
						if (check_coin_existence(TEN_CENTS, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = TEN_CENTS;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else if (strtol(token, &tokenEndPtr, 10) == FIVE_CENTS_DENOM)
					{
						if (check_coin_existence(FIVE_CENTS, system) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}

						system->cash_register[i].denom = FIVE_CENTS;
						token = strtok(NULL, COIN_DELIM);
						if (token == NULL)
						{
							fprintf(stderr, "\nError in coin file: insufficient tokens for a coin.\n");
							fclose(coins_file);
							return FALSE;
						}

						if (check_count(token) == FALSE)
						{
							fclose(coins_file);
							return FALSE;
						}
						system->cash_register[i].count = strtol(token, &tokenEndPtr, 10);
					}
					else
					{
						fprintf(stderr, "\nError in coin file: The %s denomination of currency is "
							"not a valid denomination.\n", token);
						fclose(coins_file);
						return FALSE;
					}

					token = strtok(NULL, COIN_DELIM);
					if (token != NULL)
					{
						fprintf(stderr, "\nError in coin file: invalid number of tokens.\n");
						fclose(coins_file);
						return FALSE;
					}
				}
			}
			else
			{
				fclose(coins_file);
				return FALSE;
			}
		}
		else
		{
			fclose(coins_file);
			return FALSE;
		}
	}

	fclose(coins_file);

	return TRUE;
}