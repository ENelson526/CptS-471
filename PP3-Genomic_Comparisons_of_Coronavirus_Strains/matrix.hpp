#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include "setup.hpp"

extern int h, g, match, mismatch;

enum Direction
{
	UNSET = -1,
	UP = 0,
	DIAGONAL = 1,
	LEFT = 2
};



class DP_cell
{
private:


public:
	int Sscore, Dscore, Iscore;
	Direction dir;

	DP_cell(int s, int d, int i)
	{
		this->Sscore = s;
		this->Dscore = d;
		this->Iscore = i;
		dir = UNSET;
	}

	DP_cell()
	{
		this->Sscore = this->Dscore = this->Iscore = 0;
		dir = UNSET;
	}
};

void read_config_file();
int find_max_of_three(int, int, int);
std::vector<std::vector<int>> similarity_map();
int similarity_score(int, int, int, int, int, int);
void zero_table();
void resize_table(int, int);

int fill_global_table(std::string, std::string);
void find_max_in_table(int, int, int*, int*);
int num_of_matches_from_cell(int, int);

#endif