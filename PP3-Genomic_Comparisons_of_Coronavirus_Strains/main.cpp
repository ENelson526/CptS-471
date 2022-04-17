
#include "tree.hpp"

std::unordered_map<char, int> u;
int main()
{
	std::vector<std::string> contents = read_input_files(get_input_files());
	u = read_alphabet_file(get_alphabet_file());

	buildGST(contents, u);
	

	return 0;
}