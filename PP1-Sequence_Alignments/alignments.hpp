#ifndef ALIGNMENTS_HPP_
#define ALIGNMENTS_HPP_


#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <limits.h>
#include <math.h>
#include <memory>
#include <sys/time.h>
#include <sys/resource.h>

enum Direction
{
    left = 0,
    diagonal = 1,
    up = 2
};

struct DP_cell {
    int Sscore;     // Substitution (S) score
    int Dscore;     // Deletion (D) score
    int Iscore;     // Insertion (I) score
    int value;
    int direction; // [0] = left, [1] = diagonal, [2] = up

    DP_cell(int s, int d, int i)
    {
        this->Sscore = s;
        this->Dscore = d;
        this->Iscore = i;
        this->value = 0;
    }

    DP_cell()
    {
        this->Sscore = 0;
        this->Dscore = 0;
        this->Iscore = 0;
        this->value = 0;
    }
};


void read_sequence_file(std::string);
void read_config_file(std::string);
void fill_global_table();
void fill_local_table();
int optimal_global(std::string*, std::string*);
int local_alignment(std::string*, std::string*);


#endif