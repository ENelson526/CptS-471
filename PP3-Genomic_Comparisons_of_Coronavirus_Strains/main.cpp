
#include "tree.hpp"

std::unordered_map<char, int> u;
int main()
{
	std::vector<std::string> contents = read_input_files(get_input_files());
	u = read_alphabet_file(get_alphabet_file());
	VALS::VALS(u, contents);
	Node* root = buildGST();
	Coloring(root);
	for (int i = 0; i < VALS::shortestUniqueSubstr.size(); ++i)
	{
		std::cout << i << " " << VALS::shortestUniqueSubstr[i] << std::endl;
	}
	return 0;
}