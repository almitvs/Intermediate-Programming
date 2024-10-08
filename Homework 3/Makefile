CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra

# Links together files needed to create a word_search executable
word_search: word_search.o search_functions.o
	$(CC) -o word_search word_search.o search_functions.o

# Links together files needed to create a test_search_functions executable
test: test_search_functions.o search_functions.o
	$(CC) -o test test_search_functions.o search_functions.o
	./test

# Compiles word_search.c to create word_search.o
word_search.o: word_search.c search_functions.h
	$(CC) $(CFLAGS) -c word_search.c

# Compiles test_search_functions.c to create test_search_functions.o
test_search_functions.o: test_search_functions.c search_functions.h
	$(CC) $(CFLAGS) -c test_search_functions.c

# Compiles search_functions.c to create search_functions.o
search_functions.o: search_functions.c search_functions.h
	$(CC) $(CFLAGS) -c search_functions.c

# Removes all objects and executables 
clean:
	rm -f *.o word_search test
