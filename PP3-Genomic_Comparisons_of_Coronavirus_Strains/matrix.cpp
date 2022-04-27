#include "matrix.hpp"
#include "tree.hpp"

DP_cell** table;
int rows = 0, cols = 0;
int g = -2, h = 0, match = 3, mismatch = -3;

// reads a config file. This file is not a requirement.
// If not called, default config values will be used
void read_config_file()
{
	std::string filename = "", line, delimeter = " ", token;
	std::cout << "Please enter the name of a config file: ";
	std::cin >> filename;

	std::ifstream infile(filename);
	if (infile.is_open())
	{
		for (int i = 0; i < 4; ++i)
		{
			std::getline(infile, line);
			token = line.substr(line.find(delimeter) + 1, line.length() - 1);
			switch (i)
			{
			case 0:
				match = std::stoi(token);
				break;
			case 1:
				mismatch = std::stoi(token);
				break;
			case 2:
				h = std::stoi(token);
				break;
			case 3:
				g = std::stoi(token);
				break;
			}
		}
		infile.close();
	}
	else
		std::cout << "Failed to open config file. Using default config values.\n";
}

// finds the max value between the three passed integers
int find_max_of_three(int i, int s, int d)
{
	if (i > std::max(s, d))
		return i;
	else if (s > d)
		return s;
	else
		return d;
}


void resize_table(int s_1, int s_2)
{
	rows = s_1;
	cols = s_2;
	table = new DP_cell*[s_1 + 1];
	for (int i = 0; i <= s_1; ++i)
	{
		table[i] = new DP_cell[s_2 + 1];
	}
}

void delete_table()
{
	for (int i = 0; i < rows; ++i)
	{
		delete[] table[i];
	}
	delete[] table;
}

std::vector<std::vector<int>> similarity_map()
{
	long double tree_total = 0, tree_avg = 0, matrix_total = 0, matrix_avg = 0;
	int tree_count = 0, matrix_count = 0;
	std::vector<std::vector<int>> D;
	std::vector<std::vector<long double>> times;
	D.resize(k);
	times.resize(k);
	for (int i = 0; i < D.size(); ++i)
	{
		D[i].resize(k);
		times[i].resize(k);
	}
	
	int x_1 = 0, x_2 = 0;

	shortestUniqueSubstr.resize(k);
	for (auto c : shortestUniqueSubstr)
	{
		c.first = 0;
		c.second = INT_MAX;
	}

	for (int i = 0; i < contents.size(); ++i)
	{
		for (int j = i + 1; j < contents.size(); ++j)
		{
			std::cout << "i : " << i << "\tj : " << j << std::endl;

			auto tree_start = std::chrono::high_resolution_clock::now();

			Node* root = new Node(0, nullptr, nullptr, 0, 0, i);
			root->parent = root->SL = root;
			root = build_ST(root, i);
			root = build_ST(root, j);
			Coloring(root);
			Node* deepest = find_LCS(root);

			auto tree_end = std::chrono::high_resolution_clock::now();
			tree_total += std::chrono::duration_cast<std::chrono::milliseconds>(tree_end - tree_start).count();

			std::cout << "LCS length : " << deepest->strDepth << "\n";
			std::cout << *deepest << "\n";

			auto matrix_start = std::chrono::high_resolution_clock::now();
			if (deepest->is_leaf())
			{
				for (auto p : deepest->leaf_list)
				{
					if (p.first == i)
					{
						x_1 = p.second;
					}
					else
					{
						x_2 = p.second;
					}
				}
			}
			else
			{
				for (auto p : deepest->children)
				{
					if (p)
					{
						if (p->s_id == i)
							x_1 = p->str.first - deepest->strDepth;
						else
							x_2 = p->str.first - deepest->strDepth;
					}
				}
			}
			

			D[i][j] = similarity_score(i, x_1, x_1 + deepest->strDepth, j, x_2, x_2 + deepest->strDepth);

			auto matrix_end = std::chrono::high_resolution_clock::now();
			times[i][j] = std::chrono::duration_cast<std::chrono::milliseconds>(matrix_end - matrix_start).count() + std::chrono::duration_cast<std::chrono::milliseconds>(tree_end - tree_start).count();
			matrix_total += std::chrono::duration_cast<std::chrono::milliseconds>(matrix_end - matrix_start).count();
			matrix_count++;
			tree_count++;

			delete root;
			delete deepest;
			std::cout << "END " << i << " " << j << "\n\n";
		}
	}

	std::cout << "Tree time : " << tree_total << " ms\n";
	std::cout << "Average Tree Time : " << tree_total / tree_count << " ms\n";
	std::cout << "Matrix time : " << matrix_total << " ms\n";
	std::cout << "Average Matrix Time : " << matrix_total / matrix_count << " ms\n";

	// print time matrix
	for (int i = 0; i < times.size(); ++i)
	{
		for (int j = 0; j < times[i].size(); ++j)
		{
			std::cout << times[i][j] << "\t";
		}
		std::cout << "\n";
	}
	return D;
}



// s_i  		: the string id of the first string
// x_1, y_1 	: starting and ending indexes in s_i of the LCS of s_i and s_j 
// s_j  		: string id of the second string
// x_2, y_2 	: starting and ending indexes in s_j of the LCS of s_i and s_j
int similarity_score(int s_i, int x_1, int y_1, int s_j, int x_2, int y_2)
{
	// b = length of LCS
	int b = y_1 - x_1;

	// extract prefixes before the LCS
	std::string s_i_rev = contents[s_i].substr(0, x_1), 
				s_j_rev = contents[s_j].substr(0, x_2);

	resize_table(s_i_rev.length(), s_j_rev.length());

	// reverse the prefixes
	std::reverse(s_i_rev.begin(), s_i_rev.end());
	std::reverse(s_j_rev.begin(), s_j_rev.end());
	std::cout << "Global table 1... : ";
	// Compute the global alignment between these two strings (with affine gap penalty function), where the recurrence is the same 
	// as the Needleman-Wunsch's algorithm in the forward phase. However, find the cell with the maximum score, not necessarily the last cell
	int a = fill_global_table(s_i_rev, s_j_rev);


	delete_table();


	// repeat of suffix after LCS
	std::string s_i_fwd = contents[s_i].substr(y_1),
				s_j_fwd = contents[s_j].substr(y_2);

	resize_table(s_i_fwd.length(), s_j_fwd.length());

	std::reverse(s_i_fwd.begin(), s_i_fwd.end());
	std::reverse(s_j_fwd.begin(), s_j_fwd.end());
	std::cout << "Global table 2... : ";
	int c = fill_global_table(s_i_fwd, s_j_fwd);

	delete_table();
	return a + c + b;
}


int fill_global_table(std::string s1, std::string s2)
{
	std::cout << " x : " << s1.length() + 1 << ", y : " << s2.length() + 1 << "\n";
	
	for (int i = 0; i < s1.length(); ++i)
	{
		table[i][0].Sscore = INT_MIN - h - g;
		table[i][0].Iscore = INT_MIN - h - g;
		table[i][0].Dscore = h + (i * g);
	}
	for (int i = 0; i < s2.length(); ++i)
	{
		table[0][i].Sscore = INT_MIN - h - g;
		table[0][i].Dscore = INT_MIN - h - g;
		table[0][i].Iscore = h + (i * g);
	}

	for (int i = 1; i <= s1.length(); ++i)
	{
		for (int j = 1; j <= s2.length(); ++j)
		{
			table[i][j].Sscore = find_max_of_three(table[i - 1][j - 1].Iscore, table[i - 1][j - 1].Sscore, table[i - 1][j - 1].Dscore) + ((s1[i - 1] == s2[j - 1]) ? match : mismatch);
			table[i][j].Dscore = find_max_of_three(table[i - 1][j].Dscore + g, table[i - 1][j].Sscore + h + g, table[i - 1][j].Iscore + h + g);
			table[i][j].Iscore = find_max_of_three(table[i][j - 1].Iscore + g, table[i][j - 1].Sscore + h + g, table[i][j - 1].Dscore + h + g);

			if (table[i][j].Sscore > std::max(table[i][j].Dscore, table[i][j].Iscore)) // Substitution (S)
			{
				table[i][j].dir = DIAGONAL;
			}
			else if (table[i][j].Dscore > table[i][j].Iscore) // Deletion (D)
			{
				table[i][j].dir = UP;
			}
			else    // Insertion (I)
			{
				table[i][j].dir = LEFT;
			}
		}
	}

	int max_i = 0, max_j = 0;
	find_max_in_table(s1.length(), s2.length(), &max_i, &max_j);
	

	return num_of_matches_from_cell(max_i, max_j);
}

void find_max_in_table(int s1_len, int s2_len, int* max_i, int* max_j)
{
	int max_val = -1000000;
	for (int i = 0; i <= s1_len; ++i)
	{
		for (int j = 0; j <= s2_len; ++j)
		{
			int temp = find_max_of_three(table[i][j].Sscore, table[i][j].Dscore, table[i][j].Iscore);
			if (temp > max_val)
			{
				max_val = temp;
				*max_i = i;
				*max_j = j;
			}
		}
	}
}

int num_of_matches_from_cell(int i, int j)
{
	int numMatches = 0;

	while (i != 0 && j != 0)
	{
		if (table[i][j].dir == UP)
			i--;
		else if (table[i][j].dir == LEFT)
			j--;
		else if (table[i][j].dir == DIAGONAL)
		{
			i--;
			j--;
			numMatches++;
		}
	}

	return numMatches;
}