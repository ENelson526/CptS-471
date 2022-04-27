#ifndef _SETUP_HPP
#define _SETUP_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <string>
#include <array>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <chrono>


extern std::unordered_map<char, int> ALPHA_VALS;
extern std::vector<std::string> contents;
extern std::vector<std::pair<int, int>> shortestUniqueSubstr;
extern int k, numIntNodes;

// Asks the user how many files they want to read from, takes in their names, and returns thems
std::vector<std::string> get_input_files();

// Reads in the files with the passed names and returns an vector with their contents
void read_input_files();

// Asks for and returns the name of an alphabet file from the user
std::string get_alphabet_file();

// Reads in the passed alphabet file and returns an unordered map of all characters in the alphabet
void read_alphabet_file();

#endif