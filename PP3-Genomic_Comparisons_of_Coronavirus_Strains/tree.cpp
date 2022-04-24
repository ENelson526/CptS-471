#include "tree.hpp"

std::unordered_map<char, int> VALS::ALPHA_VALS;
std::vector<std::string> VALS::contents;
std::vector<int> VALS::shortestUniqueSubstr;
int VALS::k, VALS::numIntNodes;



Node* buildGST()
{
	Node* root = new Node(VALS::numIntNodes++, nullptr, nullptr, 0, 0, 0);
	root->parent = root->SL = root;
	for (int i = 0; i < VALS::k; ++i)
	{
		build_ST(root, i);
	}
	return root;
}


void build_ST(Node* root, int s_id)
{
	Node* v, * u, * newLeaf, * v_prime, * u_prime;
	v = u = v_prime = u_prime = nullptr;

	newLeaf = findPath(root, 0, s_id);

	for (int i = 1; i < VALS::contents[s_id].length(); ++i)
	{
		//u = newLeaf->parent;
		//v = u->SL;
		
		//if (v)
		//{
			//newLeaf = findPath(v, i + v->strDepth, s_id);
		newLeaf = findPath(root, i, s_id);
		//}
		/*
		else
		{
			u_prime = u->parent;
			v_prime = u_prime->SL;

			if (u_prime != root)
			{
				u->SL = nodeHops(v_prime, VALS::contents[s_id].substr(u->str.first, u->str.second), i, s_id);
				newLeaf = findPath(u->SL, i + u->SL->strDepth, s_id);
			}
			else
			{
				std::string beta_prime = VALS::contents[u_prime->s_id].substr(u->str.first + 1, u->str.second - 1);
				u->SL = nodeHops(root, beta_prime, i, s_id);
				newLeaf = findPath(u->SL, i + u->SL->strDepth, s_id);
			}
		}*/
	}

}

Node* nodeHops(Node* v, std::string beta, int i, int s_id)
{
	if (beta == "")
		return v;

	Node* child = v->children[VALS::ALPHA_VALS[beta[0]]];

	if (child)
	{
		if (child->str.second < beta.length())
			return nodeHops(child, beta.substr(child->str.second), i + child->str.second, s_id);
		else if (child->str.second == beta.length())
			return child;
		else
		{
			int j = beta.length();
			// string for old s_id, should be broken to path of old node
			std::string childStr = VALS::contents[child->s_id].substr(child->str.first, child->str.second);

			Node* newIntNode = new Node(VALS::numIntNodes++, v, nullptr, child->str.first, j, child->s_id);
			std::memcpy(&newIntNode->children[VALS::ALPHA_VALS[childStr[j]]], &child, sizeof(Node*));

			newIntNode->children[VALS::ALPHA_VALS[childStr[j]]]->parent = newIntNode;
			newIntNode->children[VALS::ALPHA_VALS[childStr[j]]]->str = { child->str.first + j, childStr.length() - j };

			v->children[VALS::ALPHA_VALS[beta[0]]] = newIntNode;

			return newIntNode;
		}
	}
	else
		return v;
}


Node* findPath(Node* u, int i, int s_id)
{
	Node* child = u->children[VALS::ALPHA_VALS[VALS::contents[s_id][i]]];
	if (child)
	{
		int j = 0;

		// existing edge string (from child's s_id)
		std::string edgeString = VALS::contents[child->s_id].substr(child->str.first, child->str.second);

		if (VALS::contents[s_id].substr(i, edgeString.length()) == edgeString)
			return findPath(child, i + edgeString.length(), s_id);

		for (j = 1; j < edgeString.length(); ++j)
		{
			if (VALS::contents[s_id][i + j] != edgeString[j])
				break;
		}

		if (j >= edgeString.length())
			return findPath(child, i + j, s_id);
		else
		{
			Node* newIntNode = new Node(0, u, nullptr, child->str.first, j, child->s_id);
			// insert leaf node for s_id
			newIntNode->children[VALS::ALPHA_VALS[VALS::contents[s_id][j + i]]] = new Node(VALS::numIntNodes++, newIntNode, nullptr, i + j, VALS::contents[s_id].substr(i).length() - j, s_id);

			std::memcpy(&newIntNode->children[VALS::ALPHA_VALS[edgeString[j]]], &child, sizeof(Node*));

			newIntNode->children[VALS::ALPHA_VALS[edgeString[j]]]->parent = newIntNode;
			newIntNode->children[VALS::ALPHA_VALS[edgeString[j]]]->str = { child->str.first + j, edgeString.length() - j };
			u->children[VALS::ALPHA_VALS[VALS::contents[s_id][i]]] = newIntNode;

			return newIntNode->children[VALS::ALPHA_VALS[VALS::contents[s_id][j + i]]];
		}
	}
	else
	{
		// if this prefix has been inserted before
		if (u->is_leaf() && u->SL == nullptr)
		{
			u->color = VALS::k + 1; // set leaf to mixed color due to reinsert
			return u;
		}
		u->children[VALS::ALPHA_VALS[VALS::contents[s_id][i]]] = new Node(VALS::numIntNodes++, u, nullptr, i, VALS::contents[s_id].length() - i, s_id);
		return u->children[VALS::ALPHA_VALS[VALS::contents[s_id][i]]];
	}
}

void Coloring(Node* u)
{
	if (u->is_leaf())
	{
		return;
	}
	else // u is not a leaf
	{
		for (auto c : u->children)
		{
			if (c)
				Coloring(c);
		}
		// after all children have been visited
		for (auto c : u->children)
		{
			if (c)
			{
				if (c->color != u->color)
				{
					u->color = VALS::k + 1;
				}
				if (c->color != VALS::k + 1)
				{
					if (u->strDepth + 1 < VALS::shortestUniqueSubstr[c->s_id])
						VALS::shortestUniqueSubstr[c->s_id] = u->strDepth + 1;
				}
			}
		}
	}
}


