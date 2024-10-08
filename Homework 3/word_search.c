// word_search.c
// Aidan Alme
// aalme2
// D1CB50


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "search_functions.h"


/*
 * This is the HW3 main function that performs a word search.
 */
int main(int argc, char* argv[]) {

  // Check that one command line arugment for the file name was entered, else throw an error
  if (argc != 2) {
    fprintf(stdout, "Please enter a command line argument.\n");
    return 1;
  }

  // Attempt to read file data into a grid
  char grid[MAX_SIZE][MAX_SIZE] = {{'\n'}};
  int dimension = populate_grid(grid, argv[1]);

  // Exit if grid was not read successfully or was improper
  if (dimension == -1 || dimension == -2) {
    return dimension;
  }
  
  int scan_successful = 0;
  bool program_quit = false;
  
  // Scan user input words and search for them in the grid until the user enters CTRL+D
  do {
    char word[MAX_SIZE] = {'\n'};
    scan_successful = fscanf(stdin, " %s", word);
    // Return if CTRL+D was entered
    if (scan_successful == EOF) {
      program_quit = true;
    } else if (scan_successful == 1) {
      // Convert the word to lower case
      for (int i = 0; i < (int) strlen(word); i++) {
	word[i] = tolower(word[i]);
      }
      find_all(grid, dimension, word, stdout);
    }
  } while (program_quit == false);
  return 0;
}

