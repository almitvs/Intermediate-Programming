#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <functional>

// Function to get graphs and count their occurence in a text
void get_data(std::string filename, std::vector<std::string>& graphs, std::unordered_map<std::string, std::vector<std::string>>& graph_words, std::string order);

// Function to convert a string to lowercase and remove punctuation
std::string convert_lower_case(std::string word);

// Function to get the number of di/trigraphs to search for
int get_num_graphs(std::ifstream& ifile);

// Function to get and store the di/trigraphs to search for
void get_graphs(std::ifstream& ifile, std::vector<std::string>& graphs, std::unordered_map<std::string, std::vector<std::string>>& graph_words, const int num_graphs);

// Function to find the number of occurences of a graph in a text
void find_occurences(std::ifstream& ifile, std::vector<std::string>& graphs, std::unordered_map<std::string, std::vector<std::string>>& graph_words);

// Function to print in ASCII order
void print_ascii_order(std::vector<std::string>& graphs, std::unordered_map<std::string, std::vector<std::string>>& graph_words);

// Function to print in reverse ASCII order
void print_reverse_ascii_order(std::vector<std::string>& graphs, std::unordered_map<std::string, std::vector<std::string>>& graph_words);

// Function to print in frequencty order
void print_count_order(std::unordered_map<std::string, std::vector<std::string>>& graph_words);

// Function to print a graph and its list of words
void print(std::vector<std::string>& graphs, std::unordered_map<std::string, std::vector<std::string>>& graph_words);

// Comparator for pairs in order to sort by frequency
bool compare_frequency(std::pair<std::string, int>& p1, std::pair<std::string, int>& p2);

// Function to prompt the users for queries
void prompt_user(std::vector<std::string>& graphs, std::unordered_map<std::string, std::vector<std::string>>& graph_words);

// Function to check if a string is an integer
bool check_integer(std::string word);

// Function to map frquencies to lists of words
void create_frequency_map(std::unordered_map<std::string, std::vector<std::string>>& graph_words, std::unordered_map<int, std::vector<std::string>>& frequencies, std::vector<int>& freqs);

// Function to check if a graph is in the list
bool check_graph(std::string graph, std::vector<std::string>& graphs);

// Function to check if a frequency is valid
bool check_freq(int num, std::vector<int>& freqs);
