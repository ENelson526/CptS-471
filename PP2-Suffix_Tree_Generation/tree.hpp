
#ifndef _TREE_HPP
#define _TREE_HPP

#define ALPHA_LEN 4
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <cstring>
#include <vector>
#include <algorithm>

// don't store strings, store start and end indexes for substr edge represents
struct Node {
    int id;
    int strDepth;
    struct Node* children[ALPHA_LEN + 1];
    struct Node* parent;
    struct Node* SL;
    std::pair<int, int> str;

    Node() {
        id = 0;
        strDepth = 0;
        for (int i = 0; i < ALPHA_LEN + 1; i++)
            children[i] = nullptr;
        parent = nullptr;
        SL = nullptr;
        str = {0, 0};
    }

    Node(int i, struct Node* p, struct Node* s, int frs, int sec)
    {
        id = i;
        // calc string depth
        if (p)
            strDepth = p->strDepth + sec;
        parent = p;
        SL = s;
        str = {frs, sec};
        for (int i = 0; i < ALPHA_LEN + 1; i++)
            children[i] = nullptr;
    }

    // display nodes children
    void displayChildren()
    {
        for (int i = 0; i < ALPHA_LEN + 1; i++)
        {
            if (children[i])
                std::cout << "id: " << children[i]->id << " str: " << children[i]->str.first << " " << children[i]->str.second << std::endl;
        }
    }

};


// Reads the sequence file and returns the string
std::string readSequenceFile(std::string);

// Reads in alphabet file and adds all characters from the alphabet to the ALPHA_VALS unordered_map
void readAlphabetFile(std::string);

// Builds a naive ST from a given string
Node* build_ST(std::string, Node**);

// finds the node with the path of the given substring. If said node does not exist, it will be inserted and a pointer
// to the new node will be returned
Node* findPath(Node*, std::string, int);

std::string getPathLabel(Node*, std::string);

Node* nodeHops(Node*, std::string, std::string, int);

void find_matching_repeat(std::string, std::ofstream&);

std::string getBWT(Node*, std::string);

void DFS(Node* n);

#endif

