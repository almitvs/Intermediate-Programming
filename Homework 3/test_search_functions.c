// test_search_functions.c
// <STUDENT: ADD YOUR INFO HERE: name, JHED, etc.>
// Aidan Alme
// aalme2
// D1CB50


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "search_functions.h"


/* 
 * Declarations for tester functions whose definitions appear below.
 * (You will need to fill in the function definition details, at the
 * end of this file, and add comments to each one.) 
 * Additionally, for each helper function you elect to add to the 
 * provided search_functions.h, you will need to supply a corresponding
 * tester function in this file.  Add a declaration for it here, its
 * definition below, and a call to it where indicated in main.
 */
void test_file_eq();      // This one is already fully defined below.
void test_populate_grid();
void test_find_right();
void test_find_left();
void test_find_down();
void test_find_up();
void test_find_all();
void test_grid_dim_0();
void test_grid_dim_1();
void test_grid_dim_9();
void test_grid_dim_10();
void test_grid_dim_11();
void test_grid_many_cols();
void test_grid_many_rows();
void test_all_letter();
void test_file_not_found();

/*
 * Main method which calls all test functions.
 */
int main() {
  printf("Testing file_eq...\n");
  test_file_eq();
  printf("Passed file_eq test.\n\n");

  printf("Running search_functions tests...\n");
  test_populate_grid();
  test_find_right();
  test_find_left();
  test_find_down();
  test_find_up();
  test_find_all();

  /* You may add calls to additional test functions here. */

  test_grid_dim_0();
  test_grid_dim_1();
  test_grid_dim_9();
  test_grid_dim_10();
  test_grid_dim_11();
  test_grid_many_cols();
  test_grid_many_rows();
  test_all_letter();
  test_file_not_found();
  
  printf("Passed search_functions tests!!!\n");
}



/*
 * Test file_eq on same file, files with same contents, files with
 * different contents and a file that doesn't exist.
 * Relies on files test1.txt, test2.txt, test3.txt being present.
 */
void test_file_eq() {
  FILE* fptr = fopen("test1.txt", "w");
  fprintf(fptr, "this\nis\na test\n");
  fclose(fptr);

  fptr = fopen("test2.txt", "w");
  fprintf(fptr, "this\nis\na different test\n");
  fclose(fptr);

  fptr = fopen("test3.txt", "w");
  fprintf(fptr, "this\nis\na test\n");
  fclose(fptr);

  assert( file_eq("test1.txt", "test1.txt"));
  assert( file_eq("test2.txt", "test2.txt"));
  assert(!file_eq("test2.txt", "test1.txt"));
  assert(!file_eq("test1.txt", "test2.txt"));
  assert( file_eq("test3.txt", "test3.txt"));
  assert( file_eq("test1.txt", "test3.txt"));
  assert( file_eq("test3.txt", "test1.txt"));
  assert(!file_eq("test2.txt", "test3.txt"));
  assert(!file_eq("test3.txt", "test2.txt"));
  assert(!file_eq("", ""));  // can't open file
}



void test_populate_grid(){
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "grid.txt");
  printf("%d\n", dimension);
  assert(dimension == 4);
}


void test_find_right(){
  char grid[MAX_SIZE][MAX_SIZE] = {{'c', 'a', 't'}, {'c', 'a', 't'}, {'c', 'a', 't'}};
  int num_found = find_right(grid, 3, "cat", stdout);
  assert(num_found == 3);

}


void test_find_left(){
  char grid[MAX_SIZE][MAX_SIZE] = {{'c', 'a', 't'}, {'c', 'a', 't'}, {'c', 'a', 't'}};
  int num_found = find_left(grid, 3, "tac", stdout);
  assert(num_found == 3);
}


void test_find_down(){
  char grid[MAX_SIZE][MAX_SIZE] = {{'c', 'a', 't'}, {'c', 'a', 't'}, {'c', 'a', 't'}};
  int num_found = find_down(grid, 3, "ccc", stdout);
  assert(num_found == 1);
}


void test_find_up(){
  char grid[MAX_SIZE][MAX_SIZE] = {{'c', 'a', 't'}, {'c', 'a', 't'}, {'c', 'a', 't'}};
  int num_found = find_up(grid, 3, "ccc", stdout);
  assert(num_found == 1);
}


void test_find_all(){
  char grid[MAX_SIZE][MAX_SIZE] = {{'b', 'o', 'b'}, {'o', 'x', 'x'}, {'b', 'x', 'x'}};
  int num_found = find_all(grid, 3, "bob", stdout);
  assert(num_found == 4);
}

void test_grid_dim_0() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "zero.txt");
  assert(dimension == -2);
}

void test_grid_dim_1() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "one.txt");
  assert(dimension == 1);
}

void test_grid_dim_9() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "nine.txt");
  assert(dimension == 9);
}

void test_grid_dim_10() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "ten.txt");
  assert(dimension == 10);
}

void test_grid_dim_11() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "eleven.txt");
  assert(dimension == -2);
}

void test_grid_many_cols() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "long.txt");
  assert(dimension == -2);
}
void test_grid_many_rows() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "skinny.txt");
  assert(dimension == -2);
}

void test_all_letter() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  populate_grid(grid, "one.txt");
  int num_found = find_all(grid, 1, "a", stdout);
  assert(num_found == 4);
}

void test_file_not_found() {
  char grid[MAX_SIZE][MAX_SIZE] = {{'\0'}};
  int dimension = populate_grid(grid, "fake.txt");
  assert(dimension == -1);
}
