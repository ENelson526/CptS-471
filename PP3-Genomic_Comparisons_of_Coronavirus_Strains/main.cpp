
#include "tree.hpp"
#include "matrix.hpp"

std::unordered_map<char, int> u;
int main()
{
	read_input_files(get_input_files());
	u = read_alphabet_file(get_alphabet_file());

	// TASK 1
	/*
	VALS::VALS(u, contents);
	Node* root = buildGST();
	Coloring(root);
	for (int i = 0; i < VALS::shortestUniqueSubstr.size(); ++i)
	{
		std::cout << i << " " << VALS::shortestUniqueSubstr[i] << std::endl;
	}
	*/

	// Task 2
	
	std::vector<std::vector<int>> test = similarity_map();
	std::cout << "\n\n";
	for (int i = 0; i < test.size(); ++i)
	{
		for (int j = 0; j < test[0].size(); ++j)
		{
			std::cout << test[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}