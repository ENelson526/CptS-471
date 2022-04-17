#ifndef _TREE_HPP
#define _TREE_HPP

#include "setup.hpp"

class Node {
private:
	static std::unordered_map<char, int> ALPHA_VALS;

	std::array<Node*, ALPHA_LEN + 1> children;
	Node* SL, * parent;

	int sId,		// string id: indicates which string this node is a prefix of. 
					// Set to k + 1 if a prefix of more than one (k is the number of inputted strings)
		strDepth,	// Sum of all edge labels from the root to this node
		id;			// id of this node

	std::pair<int, int> str;


	bool is_leaf();
	Node* findPath(std::string, int);
	Node* nodeHops(std::string, std::string, int);

public:

	Node()
	{
		this->children = { nullptr };
		this->SL = this->parent = nullptr;
		this->sId = -1;
		this->id = -1;
		this->strDepth = -1;
	}

	Node(int id, Node* parent, Node* suffix_link, int starting_index, int length, int string_id)
	{
		this->children = { nullptr };
		this->id = id;
		this->parent = parent;
		if (this->parent)
			strDepth = this->parent->strDepth + length;

		this->SL = suffix_link;
		this->str = { starting_index, length };
		this->sId = string_id;
	}

	// Deconstructor
	~Node()
	{
		for (Node* c : this->children)
			if (c != nullptr)
				delete c;
		delete this->SL;
		delete this->parent;
	}


	// Gets the path label of this node
	std::string get_path_label(std::string);

	// Builds an ST for the string at this
	void buildST(std::string, int);



	static void set_alphabet(std::unordered_map<char, int> u)
	{
		for (auto p : u)
		{
			ALPHA_VALS.insert(p);
			std::cout << p.first << " " << p.second << "\n";
		}
	}
};


Node* buildGST(std::vector<std::string>, std::unordered_map<char, int>);



#endif