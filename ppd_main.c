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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
* @file ppd_main.c contains the main function implementation and any
* helper functions for main such as a display_usage() function.
**/

/* checks the format of the input */
BOOLEAN check_input(char * input)
{
	int i;
	char new_input[INPUT_SIZE + 2];

	memset(new_input, '\0', sizeof(new_input));
	strcpy(new_input, input);

	for (i = 0; i < strlen(new_input); i++)
	{
		if (!isdigit(new_input[i]))
		{
			fprintf(stderr, "Error: input was not a number. Please try again.\n");
			return FALSE;
		}
	}

	return TRUE;
}

/**
* manages the running of the program, initialises data structures, loads
* data and handles the processing of options. The bulk of this function
* should simply be calling other functions to get the job done.
**/
int main(int argc, char **argv)
{
	BOOLEAN test;
	
	char * stock_name, * coins_name;
	char choice[INPUT_SIZE + 2];
	char * tokenEndPtr;
	int menuChoice = 0;

	/* represents the data structures to manage the system */
	struct ppd_system system;

	struct menu_item menu[MENU_OPTIONS];

	/* validate command line arguments */
	if (argc != NUM_ARGS)
	{
		fprintf(stderr, "\nError: invalid arguments passed in.\n");
		fprintf(stderr, "Correct arguments are: \n");
		fprintf(stderr, "\t./ppd <stockfile> <coinfile>\n");
		fprintf(stderr, "Where <stockfile> and <coinfile> are two valid files"
			" in the expected format.\n\n");
		return EXIT_FAILURE;
	}

	/* init the system */
	system_init(&system);

	stock_name = argv[1];
	coins_name = argv[2];

	/* load data */
	/* test if everything has been initialised correctly */
	if (load_data(&system, coins_name, stock_name) == FALSE)
	{
		system_free(&system);
		fprintf(stderr, "Correct arguments are: \n");
		fprintf(stderr, "\t./ppd <stockfile> <coinfile>\n");
		fprintf(stderr, "Where <stockfile> and <coinfile> are two valid files"
			" in the expected format.\n\n");
		return EXIT_FAILURE;
	}

	/* initialise the menu system */
	init_menu(menu);

	do
	{
		do
		{
			/* loop, asking for options from the menu */
			display_menu(menu);

			do
			{
				printf("\nSelect your option (1-9): ");

				if (fgets(choice, INPUT_SIZE + 2, stdin) == NULL)
				{
					test = FALSE;
				}
				else if (strcmp(choice, "\n") == 0)
				{
					test = FALSE;
				}
				else
				{
					test = TRUE;
				}
			} while (test == FALSE);

			if (choice[strlen(choice) - 1] != '\n')
			{
				read_rest_of_line();
				menuChoice = (int)strtol(choice, &tokenEndPtr, 10);
				fprintf(stderr, "Error: Input is too long. Please Try Again.\n");
				test = FALSE;
			}
			else
			{
				choice[strlen(choice) - 1] = '\0';
				menuChoice = (int)strtol(choice, &tokenEndPtr, 10);
				test = TRUE;
			}
		} while (test == FALSE);
		
		test = check_input(choice);
		
		if (test != FALSE)
		{
			if (menuChoice > 0 && menuChoice < 10)
			{
				if (menuChoice != 9)
				{
					test = TRUE;
					/* run each option selected */
					if (menu[menuChoice - 1].function(&system) == FALSE)
					{
						printf("\nError: The task %s failed to run successfully.\n", menu[menuChoice - 1].name);
					}
				}
			}
			else
			{
				fprintf(stderr, "Error: %d is not a valid choice. Please Try Again.\n", menuChoice);
			}
		}
	} while (menuChoice != 9 && menuChoice != 3);
	/* until the user quits */

	/* make sure you always free all memory and close all files
	* before you exit the program
	*/
	system_free(&system);

	return EXIT_SUCCESS;
}
