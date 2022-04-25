#include "tree.hpp"

std::unordered_map<char, int> ALPHA_VALS;
std::vector<std::string> contents;
std::vector<int> shortestUniqueSubstr;
int k, numIntNodes;

int numLeafs = 0;

Node* buildGST()
{
	Node* root = new Node(numIntNodes++, nullptr, nullptr, 0, 0, 0);
	root->parent = root->SL = root;
	for (int i = 0; i < contents.size(); ++i)
	{
		build_ST(root, i);
	}
	return root;
}


Node* build_ST(Node* root, int s_id)
{
	Node* v, * u, * newLeaf, * v_prime, * u_prime;
	v = u = v_prime = u_prime = nullptr;


	newLeaf = findPath(root, 0, s_id);

	for (int i = 1; i < contents[s_id].length(); ++i)
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
				u->SL = nodeHops(v_prime, contents[s_id].substr(u->str.first, u->str.second), i, s_id);
				newLeaf = findPath(u->SL, i + u->SL->strDepth, s_id);
			}
			else
			{
				std::string beta_prime = contents[u_prime->s_id].substr(u->str.first + 1, u->str.second - 1);
				u->SL = nodeHops(root, beta_prime, i, s_id);
				newLeaf = findPath(u->SL, i + u->SL->strDepth, s_id);
			}
		}*/
	}

	return root;

}

Node* nodeHops(Node* v, std::string beta, int i, int s_id)
{
	if (beta == "")
		return v;

	Node* child = v->children[ALPHA_VALS[beta[0]]];

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
			std::string childStr = contents[child->s_id].substr(child->str.first, child->str.second);

			Node* newIntNode = new Node(numIntNodes++, v, nullptr, child->str.first, j, child->s_id);
			std::memcpy(&newIntNode->children[ALPHA_VALS[childStr[j]]], &child, sizeof(Node*));

			newIntNode->children[ALPHA_VALS[childStr[j]]]->parent = newIntNode;
			newIntNode->children[ALPHA_VALS[childStr[j]]]->str = { child->str.first + j, childStr.length() - j };

			v->children[ALPHA_VALS[beta[0]]] = newIntNode;

			return newIntNode;
		}
	}
	else
		return v;
}


Node* findPath(Node* u, int i, int s_id)
{
	Node* child = u->children[ALPHA_VALS[contents[s_id][i]]];
	if (child)
	{
		int j = 0;

		// existing edge string (from child's s_id)
		std::string edgeString = contents[child->s_id].substr(child->str.first, child->str.second);

		if (contents[s_id].substr(i, edgeString.length()) == edgeString)
		{
			if (child->is_leaf())
			{
				child->leaf_list.push_back({ s_id, i - u->strDepth });
				child->color = k + 1;
				return child;
			}
			else
				return findPath(child, i + edgeString.length(), s_id);
		}

		for (j = 1; j < edgeString.length(); ++j)
		{
			if (contents[s_id][i + j] != edgeString[j])
				break;
		}

		if (j >= edgeString.length())
			return findPath(child, i + j, s_id);
		else
		{
			Node* newIntNode = new Node(0, u, nullptr, child->str.first, j, child->s_id);
			// insert leaf node for s_id
			newIntNode->children[ALPHA_VALS[contents[s_id][j + i]]] = new Node(i, newIntNode, nullptr, i + j, contents[s_id].substr(i).length() - j, s_id);
			newIntNode->children[ALPHA_VALS[contents[s_id][j + i]]]->leaf_list.push_back({ s_id, i + j });
			std::memcpy(&newIntNode->children[ALPHA_VALS[edgeString[j]]], &child, sizeof(Node*));

			newIntNode->children[ALPHA_VALS[edgeString[j]]]->parent = newIntNode;
			newIntNode->children[ALPHA_VALS[edgeString[j]]]->str = { child->str.first + j, edgeString.length() - j };
			u->children[ALPHA_VALS[contents[s_id][i]]] = newIntNode;

			return newIntNode->children[ALPHA_VALS[contents[s_id][j + i]]];
		}
	}
	else
	{
		// if this prefix has been inserted before
		if (u->is_leaf() && u->SL == nullptr)
		{
			u->leaf_list.push_back({ s_id, i });
			u->color = k + 1; // set leaf to mixed color due to reinsert
			return u;
		}
		u->children[ALPHA_VALS[contents[s_id][i]]] = new Node(numIntNodes++, u, nullptr, i, contents[s_id].length() - i, s_id);
		u->children[ALPHA_VALS[contents[s_id][i]]]->leaf_list.push_back({ s_id, i });
		return u->children[ALPHA_VALS[contents[s_id][i]]];
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
					u->color = k + 1;
				}
				if (c->color != k + 1)
				{
					if (u->strDepth + 1 < shortestUniqueSubstr[c->s_id])
						shortestUniqueSubstr[c->s_id] = u->strDepth + 1;
				}
			}
		}
	}
}

Node* find_LCS(Node* u)
{
	Node* temp = u;
	for (auto c : u->children)
	{
		if (c)
		{
			if (c->color == k + 1)
			{
				if (c->is_leaf())
				{
					if (c->strDepth > temp->strDepth)
						temp = c;
				}
				else
				{
					Node* t = find_LCS(c);
					if (t->strDepth > temp->strDepth)
						temp = t;
				}
			}
		}
	}
	return temp;
}


/*
Node* find_LCS(Node* u)
{
	Node* temp = u;
	int i = 0, max = -1;
	for (auto c : u->children)
	{
		if (c)
		{
			if (c->color == k + 1)
			{
				if (!c->is_leaf())
					temp = find_LCS(c);
				else
				{
					if (max == -1)
						max = i;
					else if (c->strDepth > u->children[max]->strDepth)
						max = i;
				}
			}
		}
		i++;
	}
	if (max == -1)
		return temp;
	if (temp->strDepth > u->children[max]->strDepth)
		return temp;
	else
		return u->children[max];
}

*/