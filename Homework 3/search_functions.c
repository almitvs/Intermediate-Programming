// search_functions.c
// Aidan Alme
// aalme2
// D1CB50


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "search_functions.h"



/* 
 * Given a filename and a MAX_SIZExMAX_SIZE grid to fill, this function 
 * populates the grid and returns n, the actual grid dimension. 
 * If filename_to_read_from can't be opened, this function returns -1.
 * If the file contains an invalid grid, this function returns -2.
 */
int populate_grid(char grid[][MAX_SIZE], char filename_to_read_from[]){

  // Open the file
  FILE* input = fopen(filename_to_read_from, "r");

  // Throw an error if the file did not open
  if (input == NULL) {
    fprintf(stdout, "Grid file failed to open.\n");
    return -1;
  }

  int num_rows = 0;

  // Read each row in the file
  while (feof(input) == 0) {
    char row[MAX_SIZE + 1] = {'\0'};
    int row_scanned = 0;
    row_scanned = fscanf(input, " %s", row);

    // Exit if all rows are read
    if (row_scanned < 1) {
      break;
    }

    // Exit if there are too many rows
    if (num_rows + 1 > MAX_SIZE) {
      fprintf(stdout, "Invalid grid.\n");
      return -2;
    }

    // Convert letters to lowercase and check for invalid characters
    for (int i = 0; i < (int) strlen(row); i++) {
      row[i] = tolower(row[i]);
      if (row[i] < 97 || row[i] > 122) {
	fprintf(stdout, "Invalid grid.\n");
	return -2;
      }
      grid[num_rows][i] = row[i];
    }
    
    num_rows++;
  }

  // Check if there are too many or too few rows
  if (num_rows <= 0 || num_rows > MAX_SIZE) {
    fprintf(stderr, "Invalid grid.\n");
    return -2;
  }

  // Check if the number of colums matches the number of rows or not
  for (int i = 0; i < num_rows; i++) {
    if (num_rows > 9 && num_rows != (int) sizeof(grid[i])) {
      fprintf(stdout, "Invalid grid.\n");
      return -2;
    } else if (num_rows < MAX_SIZE && num_rows != (int) strlen(grid[i])) {
      fprintf(stdout, "Invalid grid.\n");
      return -2;
    }
  }
  
  // Close the file and return the dimensions of the grid
  fclose(input);
  return num_rows;
}


/* 
 * Given a grid of characters this functions finds a given word going from the left and returns the total found
 */
int find_left(char grid[][MAX_SIZE], int n, char word[], FILE *write_to){
  
  int num_found = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // Trigger a search if the first letter is found
      if (grid[i][j] == word[0]) {
        int word_index = 1;
	// Check for the rest of the letters
        for (int k = j - 1; k > j - (int) strlen(word) && k > -1; k--) {
          if (grid[i][k] == word[word_index]) {
            word_index++;
	  } else {
	    break;
	  }
        }
	// Print if the word has been found
        if (word_index == (int) strlen(word))	{
          fprintf(write_to, "%s %d %d L\n", word, i, j);
          num_found++;
        }
      }
    }
  }
  return num_found;
}


/* 
 * Given a grid of characters this functions finds a given word going from the right and returns the total found
 */
int find_right (char grid[][MAX_SIZE], int n, char word[], FILE *write_to){

  int num_found = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // Trigger a search if the first letter is found
      if (grid[i][j] == word[0]) {	
        int word_index = 1;
	// Check for the rest of the letters
	for (int k = j + 1; k < j + (int) strlen(word) && k < n; k++) {
	  if (grid[i][k] == word[word_index]) {
	    word_index++;
	  } else {
	    break;
	  }
	}
	// Print if the word has been found
	if (word_index == (int) strlen(word)) {
	  fprintf(write_to, "%s %d %d R\n", word, i, j);
	  num_found++;
	}
      }
    }
  }
  return num_found;
}


/* 
 * Given a grid of characters this functions finds a given word going down and returns the total found
 */
int find_down (char grid[][MAX_SIZE], int n, char word[], FILE *write_to){

  int num_found = 0;
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i++) {
      // Trigger a search if the first letter is found
      if (grid[i][j] == word[0]) {
        int word_index = 1;
	//Check for the rest of the letters
	for (int k = i + 1; k < i + (int) strlen(word) && k < n; k++) {
	  if (grid[k][j] == word[word_index]) {
	    word_index++;
	  } else {
	    break;
	  }
	}
	// Print if the word has been found
        if (word_index == (int) strlen(word)) {
          fprintf(write_to, "%s %d %d D\n", word, i, j);
          num_found++;
        }
      }
    }
  }
  return num_found;
}


/* 
 * Given a grid of characters this functions finds a given word going up and returns the total found
 */
int find_up   (char grid[][MAX_SIZE], int n, char word[], FILE *write_to){
  
  int num_found = 0;
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i++) {
      // Trigger a search if the first letter is found
      if (grid[i][j] == word[0]) {
        int word_index = 1;
	// Check for the rest of the letters
        for (int k = i - 1; k > i - (int) strlen(word) && k > -1; k--) {
          if (grid[k][j] == word[word_index]) {
            word_index++;
          } else {
	    break;
	  }
        }
	// Print if the word has been found
        if (word_index == (int) strlen(word)) {
          fprintf(write_to, "%s %d %d U\n", word, i, j);
          num_found++;
        }
      }
    }
  }
  return num_found;

}


/* 
 * Given a grid of characters this functions finds a given word going all directions and returns the total found
 */
int find_all  (char grid[][MAX_SIZE], int n, char word[], FILE *write_to) {

  int num_found = 0;
  num_found += find_right(grid, n, word, write_to);
  num_found += find_left(grid, n, word, write_to);
  num_found += find_down(grid, n, word, write_to);
  num_found += find_up(grid, n, word, write_to);

  if (num_found == 0) {
    fprintf(write_to, "%s - Not Found\n", word);
  }
  
  return num_found;
} 


/*
 * Reads lhs and rhs character by character until either reaches eof.
 * Returns true if the files can be read and the two files match
 * character by character. Returns false if two files either can't be
 * opened or don't match. The definition of this function is provided 
 * for you.
 */
int file_eq(char lhs_name[], char rhs_name[]) {
  FILE* lhs = fopen(lhs_name, "r");
  FILE* rhs = fopen(rhs_name, "r");

  // don't compare if we can't open the files
  if (lhs == NULL || rhs == NULL) return 0;

  int match = 1;
  // read until both of the files are done or there is a mismatch
  while (!feof(lhs) || !feof(rhs)) {
	if (feof(lhs) ||                      // lhs done first
		feof(rhs) ||                  // rhs done first
		(fgetc(lhs) != fgetc(rhs))) { // chars don't match
	  match = 0;
	  break;
	}
  }
  fclose(lhs);
  fclose(rhs);
  return match;
}

