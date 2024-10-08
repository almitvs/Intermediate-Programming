#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "digraph_functions.h"
#include <functional>

using std::endl;
using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;
using std::ifstream;
using std::pair;
using std::greater;

int main(int argc, char * argv[]) {
  // if number of arguments is not correct, exit with code 1!
  if (argc != 3) {
    cout<<"Invalid arguments";
    return 1;
  }

  // if the "order" argument is not valid, exit with code 1!
  string order(argv[2]);
  if (order != "r" && order != "a" && order != "c") {
    cout<<"Invalid arguments";
    return 1;
  }

  // Get the filename
  string filename = argv[1];

  // Create a vector to store the di/trigraphs
  vector<string> graphs;

  // Create an unordered map to store the di/trigraphs and vectors of words in which they occur
  unordered_map<string, vector<string>> graph_words;
  
  // Get the data and print the results
  get_data(filename, graphs, graph_words, order);

  // Let the user enter prompt until they enter "exit"
  prompt_user(graphs, graph_words);
    
  return 0;
}


