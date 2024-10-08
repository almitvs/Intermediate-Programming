/*
 * Program to represent a collection of coins of different denomination input
 * by the user
 * Intermediate Programming HW1
 * D1CB50
 */

#include <stdio.h>
#include <stdbool.h>

int main(void){

  //Arrays to store identifier symbols, their values, and their quanities
  const int MAX_DENOMINATIONS = 20;
  char identifiers[MAX_DENOMINATIONS];
  int values[MAX_DENOMINATIONS];
  int quantities[MAX_DENOMINATIONS];
  
  //Initialize the quantities to 0
  for (int i = 0; i < MAX_DENOMINATIONS; i++) {
    quantities[i] = 0;
  }

  //The command do-while loop will end when this is true
  bool program_quit = false;

  //Prompt user to enter number of  denominations
  printf("How many denominations? ");
  int num_denominations;
  int input = scanf(" %i", &num_denominations);

  //Exit if input is invalid
  if (input != 1) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }

  //Prompt user for denomination identifiers and their values
  for(int i = 0; i < num_denominations; i++){
    printf("Enter coin identifier and value in cents: ");
    int input = scanf(" %c%i", &identifiers[i], &values[i]);
    //Exit if input is invalid
    if (input != 2) {
      fprintf(stderr, "Invalid input\n");
      return 1;
    }
  }

  //Prompt user to enter a command; repeats until user enters q or uses ctrl+d
  int command_input = 0;
  do {
    //Prompts user for command
    printf("Enter a command: ");
    char command;
    command_input = scanf(" %c", &command);

    //Exits if user uses ctrl+d
    if (command_input == EOF) {
      break;
    }

    //Exits if input is invalid
    if (command_input != 1) {
      fprintf(stderr, "Invalid command\n");
      return 3;
    }

    //Executes commands
    switch(command){

    //Adds a quantity of a given denomination
    case 'a': {
      char identifier;
      int quantity;
      int input = scanf(" %c%i", &identifier, &quantity);

      //Exits if input is invalid or not properly read
      if (input != 2) {
        fprintf(stderr, "Invalid input\n");
        return 1;
      }
      int i;

      //Searches for then increases the quanitity
      for (i = 0; i < num_denominations; i++) {
	if (identifiers[i] == identifier) {
	  quantities[i] += quantity;
	  break;
	}
      }

      //Exits if an invalid demonination was entered
      if (i == num_denominations) {
	fprintf(stderr, "Unknown coin identifier\n");
	return 2;
      }
      break;
    }

    //Subtracts a quantity of a given denomination
    case 'r': {
      char identifier;
      int quantity;
      int input = scanf(" %c%i", &identifier, &quantity);

      //Exits if input is invalid or not properly read
      if (input != 2) {
        fprintf(stderr, "Invalid input\n");
        return 1;
      }
      int i;

      //Searchs for then increases the quanity
      for (i = 0; i < num_denominations; i++) {
	if (identifiers[i] == identifier) {
          quantities[i]	-= quantity;
          break;
        }
      }

      //Exits if an impromper denomination was entered
      if (i == num_denominations) {
        fprintf(stderr, "Unknown coin identifier\n");
	return 2;
      }
      break;
    }

    //Prints out the identifiers, values, and counts in csv format with a header and total value at the end
    case 's': {
      double total_value = 0;
      printf("Identifier,Face Value,Count,Total Value\n");
      for (int i = 0; i < num_denominations; i++) {
	total_value += values[i] * quantities[i];
	printf("%c,%i,%i,%i\n", identifiers[i], values[i], quantities[i], values[i] * quantities[i]);
      }
      printf("Overall value of collection: $%.2f\n", total_value / 100);
      break;
    }

    //Quits the program 
    case 'q': {
      program_quit = true;
      break;
    }

    //Invalid input cvauses the program to exit
    default: {
      fprintf(stderr, "Invalid command\n");
      return 3;
    }
    }
  } while (!program_quit);

  //Prints a goodbye message
  printf("Bye!\n");
  return 0;
}
