#ifndef _TREE_HPP
#define _TREE_HPP

#include "setup.hpp"



static class VALS {

private:
	void set_k()
	{
		k = contents.size();
	}

	void set_alphabet(std::unordered_map<char, int> u)
	{
		for (auto p : u)
			ALPHA_VALS.insert(p);
	}

	void set_contents(std::vector<std::string> c)
	{
		for (auto s : c)
			contents.push_back(s);
	}

public:
	static std::unordered_map<char, int> ALPHA_VALS;
	static std::vector<std::string> contents;
	static std::vector<int> shortestUniqueSubstr;
	static int k, numIntNodes;



	VALS(std::unordered_map<char, int> u, std::vector<std::string> c)
	{
		set_alphabet(u);
		set_contents(c);

		set_k();
		numIntNodes = 0;

		shortestUniqueSubstr.resize(k);
		for (int i = 0; i < k; ++i)
			shortestUniqueSubstr[i] = INT_MAX;
	}
};

class Node
{

private:



public:
	int id, s_id, strDepth, color;
	std::vector<Node*> children;
	Node* SL, * parent;

	std::pair<int, int> str;

	Node()
	{
		this->id = 0;
		this->strDepth = 0;
		for (int i = 0; i < VALS::k; ++i)
			this->children.push_back(nullptr);
		
		this->parent = this->SL = nullptr;
		this->str = { 0, 0 };
	}

	Node(int i, Node* parent, Node* sl, int start, int len, int s_id)
	{
		this->id = i;
		this->parent = parent;
		this->SL = sl;
		this->s_id = this->color = s_id;
		if (parent)
			this->strDepth = parent->strDepth + len;
		this->str = { start, len };
		
		for (int i = 0; i < VALS::ALPHA_VALS.size(); ++i)
			this->children.push_back(nullptr);
	}


	// if a node has a suffix link, it is an internal node
	bool is_leaf()
	{
		for (auto p : this->children)
		{
			if (p != nullptr)
				return false;
		}
		return true;
	}



};

Node* buildGST();
Node* nodeHops(Node* v, std::string beta, int i, int s_id);
Node* findPath(Node* u, int i, int s_id);
void build_ST(Node* root, int s_id);
void Coloring(Node* u);

#endif