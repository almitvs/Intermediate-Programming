#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include "digraph_functions.h"
#include <unordered_map>
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

/*
 * Master function to gather the di/trigraphs into a vector and map and count the frequencies at which they occur in the text
 */
void get_data(string filename, vector<string>& graphs, unordered_map<string, vector<string>>& graph_words, string order) {
  // Open the text file
  ifstream ifile(filename);

  // Read and store the number of di/trigraphs
  int num_graphs = get_num_graphs(ifile);

  // Read and store the di/trigraph to search for
  get_graphs(ifile, graphs, graph_words, num_graphs);

  // Store the words in which the di/trigraphs occur in the text
  find_occurences(ifile, graphs, graph_words);
  // Print the results in the proper order
  if (order == "a") {
    print_ascii_order(graphs, graph_words);
  } else if (order == "r") {
    print_reverse_ascii_order(graphs, graph_words);
  } else if (order == "c") {
    print_count_order(graph_words);
  }

  return;
}

/*
 * Function to convert a word to lower case
 */
string convert_lower_case(string word) {
  string lower;
  for (int i = 0; i < (int) word.length(); i++) {
    if (isalpha(word.at(i))) {
      lower += (char) tolower(word.at(i));
    }
  }
  return lower;
}

/*
 * Function to get the number of di/trigraphs in the text file to search for
 */
int get_num_graphs(ifstream& ifile) {
  string number;
  ifile >> number;
  return stoi(number);
}

/*
 * Function to get the di?trigraphs to search for in the text file
 */
void get_graphs(ifstream& ifile, vector<string>& graphs, unordered_map<string, vector<string>>& graph_words, const int num_graphs) {
  for (int i = 0; i < num_graphs; i++) {
    string graph;
    ifile >> graph;

    // Convert the string to lowercase
    graph = convert_lower_case(graph);
    vector<string> words;

    // Store in the data structures
    graph_words[graph] = words;
    graphs.push_back(graph);
  }
  return;
}

/*
 * Function to find the occurences of a set of di/trigraphs in a given text
 */
void find_occurences(ifstream& ifile, vector<string>& graphs, unordered_map<string, vector<string>>& graph_words) {
  string word;
  while (ifile >> word) {
    // Convert the words in the text to lowercase to match the lowercase of the graphs
    word = convert_lower_case(word);
    for (vector<string>::const_iterator it = graphs.cbegin(); it != graphs.cend(); ++it) {
      if ((int) word.find(*it) != -1) {
	// Store the words
        graph_words[*it].push_back(word);
      }
    }
  }
  return;
}

/*
 * Function to print the graphs and the words they occur in in ASCII order
 */
void print_ascii_order(vector<string>& graphs, unordered_map<string, vector<string>>& graph_words) {
  sort(graphs.begin(), graphs.end());
  print(graphs, graph_words);
  return;
}

/*
 * Function to print the graphs and the words they occur in in reverse ASCII order
 */
void print_reverse_ascii_order(vector<string>& graphs, unordered_map<string, vector<string>>& graph_words) {
  sort(graphs.begin(), graphs.end(), greater<string>());
  print(graphs, graph_words);
  return;
}

/*
 * Function to print the graphs and the words they occur in in order of frequency descending   
 */
void print_count_order(unordered_map<string, vector<string>>& graph_words) {
  vector<pair<string, int>> count_order;
  // Create a vector of key value pairs
  for (unordered_map<string, vector<string>>::const_iterator it = graph_words.cbegin(); it != graph_words.cend(); ++it) {
    pair<string, int> new_pair = {it->first, it->second.size()};
    count_order.push_back(new_pair);
  }
  // Sort the vector by the value (frequency)
  sort(count_order.begin(), count_order.end(), compare_frequency);
  // Print the list
  for (vector<pair<string, int>>::const_iterator ite = count_order.cbegin(); ite != count_order.cend(); ++ite) {
    cout << ite->first << ": " << "[";
    // Print a list of words if it is not empty                                                                                                                                                               
    if ((int) graph_words[ite->first].size() > 0) {
      for (vector<string>::const_iterator iter = graph_words[ite->first].cbegin(); iter != graph_words[ite->first].cend(); ++iter) {
	cout << *iter;
	if (iter != graph_words[ite->first].cend() - 1) {
	  cout << ", ";
	}
      }
    }
    cout << "]" << endl;
  }
  return;
}

/*
 * Function to print a graph and its vector of words
 */
void print(vector<string>& graphs, unordered_map<string, vector<string>>& graph_words) {
  for (vector<string>::const_iterator it = graphs.cbegin(); it != graphs.cend(); ++it) {
    cout << *it << ": " << "[";
    // Print a list of words if it is not empty
    if ((int) graph_words[*it].size() > 0) {
      for (vector<string>::const_iterator ite = graph_words[*it].cbegin(); ite != graph_words[*it].cend(); ++ite) {
	cout << *ite;
	if (ite < graph_words[*it].cend() - 1) {
	  cout << ", ";
	}
      }
    }
    cout << "]" << endl;
  }
  return;
}

/*
 * Comparator to compare key value pairs by value
 */
bool compare_frequency(pair<string, int>& p1, pair<string, int>& p2) {
  if (p1.second > p2.second) {
    return true;
  } else if (p1.second == p2.second) {
    if (p1.first < p2.first) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

/*
 * Function to prompt the user to enter a query
 */
void prompt_user(vector<string>& graphs, unordered_map<string, vector<string>>& graph_words) {
  bool program_quit = false;
  string query;
  unordered_map<int, vector<string>> frequencies;
  vector<int> freqs;
  create_frequency_map(graph_words, frequencies, freqs);
  
  // Prompt the user until they enter exit
  while (program_quit == false) {
    cout << "q?>";
    cin >> query;
    // Check if the query is a number
    if (check_integer(query)) {
      int num = stoi(query);
      if (check_freq(num, freqs)) {
	print(frequencies[num], graph_words);
      } else {
	cout << "None" << endl;
      }
    } else {
      // Handle the query as a digraph
      query = convert_lower_case(query);
      if (query == "exit") {
	program_quit = true;
      } else {
	if (check_graph(query, graphs)) {
	  cout << graph_words[query].size() << ": " << "[";
	  // Print a list of words if it is not empty                                                                                                                                                         
	  if ((int) graph_words[query].size() > 0) {
	    for (vector<string>::const_iterator it = graph_words[query].cbegin(); it != graph_words[query].cend(); ++it) {
	      cout << *it;
	      if (it < graph_words[query].cend() - 1) {
		cout << ", ";
	      }
	    }
	  }
	  cout << "]" << endl;
	} else {
	  // Quit
	  cout << "No such digraph" << endl;
	}
      }
    }
  }
}

/*
 * Function to check if a string is an integer
 */
bool check_integer(string word) {
  for (int i = 0; i < (int) word.length(); i++) {
    if (!isdigit(word.at(i))) {
      return false;
    }
  }
  return true;
}

/*
 * Function to get an unordered map of frequencies and graphs that occur this many times
 */
void create_frequency_map(unordered_map<string, vector<string>>& graph_words, unordered_map<int, vector<string>>& frequencies, vector<int>& freqs) {
  // Map graphs to their frequencies
  for (unordered_map<string, vector<string>>::const_iterator it = graph_words.cbegin(); it != graph_words.cend(); ++it) {
    frequencies[(int) graph_words[it->first].size()].push_back(it->first);
    freqs.push_back(graph_words[it->first].size());
  }
  // Sort the lists of graphs
  for (unordered_map<int, vector<string>>::iterator ite = frequencies.begin(); ite != frequencies.end(); ++ite) {
    sort((ite->second).begin(), (ite->second).end());
  }
  return;
}

/*
 * Function to check is a graph is valid
 */
bool check_graph(string graph, vector<string>& graphs) {
  for (vector<string>::const_iterator it = graphs.cbegin(); it != graphs.cend(); ++it) {
    if (*it == graph) {
      return true;
    }
  }
  return false;
}

/*                                                                                                                                                                                                             
 * Function to check is a frequency is valid                                                                                                                                                                  
 */
bool check_freq(int num, vector<int>& freqs) {
  for (vector<int>::const_iterator it = freqs.cbegin(); it != freqs.cend(); ++it) {
    if (*it == num) {
      return true;
    }
  }
  return false;
}
