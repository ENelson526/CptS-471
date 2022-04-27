
#include "tree.hpp"
#include "matrix.hpp"


int main()
{
	read_input_files();
	read_alphabet_file();

	// TASK 1
	std::cout << "\n\n---------------------- Task 1 ----------------------\n\n";
	auto start_fingerprint = std::chrono::high_resolution_clock::now(), start_GST = std::chrono::high_resolution_clock::now();

	Node* root = buildGST();

	auto end_GST = std::chrono::high_resolution_clock::now();


	// Init shortestUniqueSubstr vector
	shortestUniqueSubstr.resize(k);
	for (int i = 0; i < k; ++i)
	{
		shortestUniqueSubstr[i].first = 0;
		shortestUniqueSubstr[i].second = INT_MAX; // set to into max value, this will be updated in the Coloring function
	}
	
	auto start_coloring = std::chrono::high_resolution_clock::now();

	// Color tree and find the shortest unique substrings
	Coloring(root);
	std::cout << "Shortest Unique Substrings: " << std::endl;
	for (int i = 0; i < shortestUniqueSubstr.size(); ++i)
	{
		std::cout << i << " " << shortestUniqueSubstr[i].second << std::endl;
	}
	print_fingerprints_to_file();

	delete root;

	auto end_coloring = std::chrono::high_resolution_clock::now();
	auto end_fingerprint = std::chrono::high_resolution_clock::now();
	
	std::cout << "Total Time for Task 1 : " << std::chrono::duration_cast<std::chrono::milliseconds>(end_fingerprint - start_fingerprint).count() << " ms\n";
	std::cout << "Total Time for Task 1 (GST): " << std::chrono::duration_cast<std::chrono::milliseconds>(end_GST - start_GST).count() << " ms\n";
	std::cout << "Total Time for Task 1 (Coloring): " << std::chrono::duration_cast<std::chrono::milliseconds>(end_coloring - start_coloring).count() << " ms\n";

	// Task 2
	std::cout << "\n\n---------------------- Task 2 ----------------------\n\n";
	std::vector<std::vector<int>> test = similarity_map();
	std::cout << "\n\nSimilarity matrix : \n";
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