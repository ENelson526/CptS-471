#ifndef _SETUP_HPP
#define _SETUP_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <string>
#include <array>

// Global constant for the length of the alphabet. 
// If longer than the alphabet, code should work, but will take more memory than needed.
// If shorter than the alphabet, code will not work
#define ALPHA_LEN 4

// Asks the user how many files they want to read from, takes in their names, and returns thems
std::vector<std::string> get_input_files();

// Reads in the files with the passed names and returns an vector with their contents
std::vector<std::string> read_input_files(std::vector<std::string>);

// Asks for and returns the name of an alphabet file from the user
std::string get_alphabet_file();

// Reads in the passed alphabet file and returns an unordered map of all characters in the alphabet
std::unordered_map<char, int> read_alphabet_file(std::string a_file);

#endif