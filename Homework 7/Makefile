CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

cTreeTest: CTreeTest.o CTree.o
	g++ -o cTreeTest CTreeTest.o CTree.o

CTreeTest.o: CTreeTest.cpp CTree.h
	g++ -c CTreeTest.cpp $(CFLAGS)

CTree.o: CTree.cpp CTree.h
	g++ -c CTree.cpp $(CFLAGS)

tTreeTest: TTreeTest.o
	g++ -o tTreeTest TTreeTest.o

TTreeTest.o: TTreeTest.cpp Tree.inc Tree.h
	g++ -c TTreeTest.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o main?  *~
