#include "tree.hpp"

std::unordered_map<char, int> Node::ALPHA_VALS;


bool Node::is_leaf()
{
	for (auto n : this->children)
	{
		if (n)
			return false;
	}
	return true;
}

Node* buildGST(std::vector<std::string> contents, std::unordered_map<char, int> u)
{
	Node* root = new Node(0, nullptr, nullptr, 0, 0, 1);
	root->set_alphabet(u);

	//for (int i = 0; i < contents.size(); ++i)
	//{
		root->buildST(contents[0], 1);
	//}
	return root;
}


void Node::buildST(std::string s, int s_id)
{
	int i = 0;
	Node* newLeaf, * u, * v, *u_prime, *v_prime;
	

	// if this is the first time through, init root
	if (s_id == 1)
	{
		this->parent = this->SL = this;
		this->strDepth = 0;
	}
	u = this, v = this;
	s += "$";
	newLeaf = this->findPath(s, i++);

	for (; i < s.length(); ++i)
	{
		u = newLeaf->parent;
		v = u->SL;

		if (v)
		{
			newLeaf = v->findPath(s, i + v->strDepth);
		}
		else
		{
			u_prime = u->parent;
			v_prime = u_prime->SL;

			if (u_prime != this)
			{
				u->SL = v_prime->nodeHops(s.substr(u->str.first, u->str.second), s, i);
				newLeaf = u->SL->findPath(s, i + u->SL->strDepth);
			}
			else
			{
				u->SL = this->nodeHops(s.substr(u->str.first + 1, u->str.second - 1), s, i);
				newLeaf = u->SL->findPath(s, i + u->SL->strDepth);
			}
		}
	}
}

Node* Node::nodeHops(std::string beta, std::string s, int i)
{
	Node* child = this->children[ALPHA_VALS[beta[0]]];

	if (child)
	{
		if (child->str.second < beta.length())
		{
			return child->nodeHops(beta.substr(child->str.second), s, i + child->str.second);
		}
		else if (child->str.second == beta.length())
			return child;
		else
		{
			int j = 0;
			std::string childStr = s.substr(child->str.first, child->str.second);

			j = beta.length();

			Node* newIntNode = new Node(0, this, nullptr, child->str.first, j, -1);
			std::memcpy(&newIntNode->children[this->ALPHA_VALS[childStr[j]]], &child, sizeof(Node*));
			newIntNode->children[ALPHA_VALS[childStr[j]]]->parent = newIntNode;
			newIntNode->children[ALPHA_VALS[childStr[j]]]->str = { child->str.first + j, childStr.length() - j };
			this->children[ALPHA_VALS[beta[0]]] = newIntNode;

			return newIntNode;
		}
	}
	else
	{
		return this;
	}
}

Node* Node::findPath(std::string s, int i)
{
	std::string x = s.substr(i);

	if (this->children[ALPHA_VALS[x[0]]])
	{
		int j = 0;
		
		std::string edgeString = s.substr(this->children[ALPHA_VALS[x[0]]]->str.first, this->children[ALPHA_VALS[x[0]]]->str.second);

		if (x.substr(0, edgeString.length()) == edgeString)
		{
			return this->children[ALPHA_VALS[x[0]]]->findPath(s, i + edgeString.length());
		}

		for (j = 1; j < edgeString.length(); ++j)
		{
			if (x[j] != edgeString[j])
				break;
		}

		if (j >= edgeString.length())
			return this->children[ALPHA_VALS[x[0]]]->findPath(s, i + j);
		else
		{
			Node* newIntNode = new Node(0, this, nullptr, this->children[ALPHA_VALS[x[0]]]->str.first, j, -1);
			newIntNode->children[ALPHA_VALS[x[j]]] = new Node(0, newIntNode, nullptr, i + j, x.length() - j, -1);
			std::memcpy(&newIntNode->children[ALPHA_VALS[edgeString[j]]], &this->children[ALPHA_VALS[x[0]]], sizeof(Node*));

			newIntNode->children[ALPHA_VALS[edgeString[j]]]->parent = newIntNode;
			newIntNode->children[ALPHA_VALS[edgeString[j]]]->str = { this->children[ALPHA_VALS[x[0]]]->str.first + j, edgeString.length() - j };
			this->children[ALPHA_VALS[x[0]]] = newIntNode;

			// return new leaf
			return newIntNode->children[ALPHA_VALS[x[j]]];
		}
	}
	else
	{
		this->children[ALPHA_VALS[x[0]]] = new Node(0, this, nullptr, i, x.length(), -1);
		return this->children[ALPHA_VALS[x[0]]];
	}

}





std::string Node::get_path_label(std::string s)
{
	return s.substr(0, this->str.first + this->str.second);
}


/*

Node* buildST(std::string s, Node* root, Node** deepestInternal)
{
	int i = 0;
	// Add '$' to end of s. Signifies the end
	s += "$";

	Node* u, * v, * newLeaf = nullptr, * v_prime, * u_prime;

	// if this node does not have a parent, it is the root
	if (root == nullptr)
	{
		root = new Node(0, nullptr, nullptr, 0, 0, 1);
		root->parent = root->SL = root;
		root->strDepth = 0;
		newLeaf = findPath(root, s, i);
		i++;
	}


	for (; i < s.length(); ++i)
	{
		u = newLeaf->parent;
		v = u->SL;

		if (v) // Case I - SL(u) is known
		{
			newLeaf = findPath(v, s, i + v->strDepth);
		}
		else // Case II - SL(u) is unknown
		{
			u_prime = u->parent;
			v_prime = u_prime->SL;

			if (u_prime != root) // Case IIA - SL(u) is unknown and u' is not the roo
			{
				u->SL = nodeHops(v_prime, s.substr(u->str.first, u->str.second), s, i);
				newLeaf = findPath(u->SL, s, i + u->SL->strDepth);
			}
			else // Case IIB - SL(u) is unknown and u' is the root
			{
				u->SL = nodeHops(root, s.substr(u->str.first + 1, u->str.second - 1), s, i);
				newLeaf = findPath(u->SL, s, i + u->SL->strDepth);
			}
		}

		if (newLeaf->parent->strDepth > (*deepestInternal)->strDepth)
			*deepestInternal = newLeaf->parent;
	}

}
*/

