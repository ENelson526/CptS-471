#ifndef _TREE_HPP
#define _TREE_HPP

#include "setup.hpp"

class Node
{

private:

public:
	int id, s_id, strDepth, color;
	std::vector<Node*> children;
	Node* SL, * parent;

	std::pair<int, int> str;
	std::vector<std::pair<int,int>> leaf_list;

	Node()
	{
		this->id = 0;
		this->strDepth = 0;
		for (int i = 0; i < ALPHA_VALS.size(); ++i)
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
		else
			this->strDepth = 0;
		this->str = { start, len };
		
		for (int i = 0; i < ALPHA_VALS.size(); ++i)
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

	friend std::ostream& operator<<(std::ostream& os, Node const & n)
	{
		return os << "Node : " << n.id << " " << n.str.first << " " << n.str.second << " " << n.strDepth << " " << n.s_id << " " << n.color;
	}

};

Node* buildGST();
Node* nodeHops(Node* v, std::string beta, int i, int s_id);
Node* findPath(Node* u, int i, int s_id);
Node* build_ST(Node* root, int s_id);
void Coloring(Node* u);
Node* find_LCS(Node*);

#endif