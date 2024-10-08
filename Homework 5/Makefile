CC=g++
CFLAGS=-g -std=c++11 -Wall -Wextra -pedantic

# Link together files to create the digraph_analyzer executable
digraph_analyzer: digraph_analyzer.o digraph_functions.o
	$(CC) -o digraph_analyzer digraph_analyzer.o digraph_functions.o

# Compiles digraph_analyzer.cpp to create digraph_analyzer.o
digraph_analyzer.o: digraph_analyzer.cpp digraph_functions.h
	$(CC) $(CFLAGS) -c digraph_analyzer.cpp

# Compiles digraph_functions.cpp to create digraph_functions.o 
digraph_functions.o: digraph_functions.cpp digraph_functions.h
	$(CC) $(CFLAGS)	-c digraph_functions.cpp

# Removes all objects and exectuables
clean:
	rm -f *.o digraph_analyzer
