#include "tree.hpp"

std::string readSequenceFile(std::string filename)
{
    std::ifstream infile;
    infile.open(filename);
    std::string line, s;
    if (infile.is_open()) // if file could open
    {
        // throw away first line
        std::getline(infile, line); // throw away the beginning information line
        
        // read every line and add to string s
        while (std::getline(infile, line))
            s += line;
       
        infile.close();
    }
    else // if file was unable to open
    {
        std::cout << "Could not open sequence file... Exiting.\n";
        exit(1);
    }

    return s;
}


void readAlphabetFile(std::string filename)
{
    extern std::unordered_map<char, int> ALPHA_VALS;
    std::ifstream infile;
    std::string line;
    infile.open(filename);
    if (infile.is_open())
    {
        ALPHA_VALS.insert(std::pair<char, int>('$', 0));
        std::getline(infile, line);
        
        // add each nonspace character to ALPHA_VALS in order they are listed. 
        // since this is an alphabet, there should not be any repeating characters
        for (char c : line)
        {
            if (c != ' ')
            {
                ALPHA_VALS.insert(std::pair<char, int>(c, ALPHA_VALS.size()));
            }
        }
    }
    else
    {
        std::cout << "Could not open alphabet file... Exiting.\n";
        exit(1);
    }

}


Node* build_ST(std::string s, Node** deepestIntNode)
{
    // import external global variables
    extern int numIntNodes, numComparisons;
    extern std::unordered_map<char, int> ALPHA_VALS;
    Node* root = new Node(numIntNodes++ , nullptr, nullptr, 0, 0);       // create root node
    root->parent = root->SL = root;                                     // sets the roots parent and SL pointer to itself
    root->strDepth = 0;                                                 // since parent is passed as nullptr, constructor will not set strDepth
    s += "$"; // add '$' at end of string
    Node* u = root, * v = root, *newLeaf = nullptr, *v_prime, *u_prime;
    int i = 0, maxStrDepth = 0;
    int IIA_executions = 0, IIB_executions = 0;
    long long IIA_dur = 0, IIB_dur = 0;
    newLeaf = findPath(root, s, i);
    std::string beta_prime = "";

    for (i = 1; i < s.length(); ++i)
    {
        u = newLeaf->parent;
        v = u->SL;
        if (v) // Case I - SL(u) is known
        {
            newLeaf = findPath(v, s, i + v->strDepth);
        }
        else // Case II - SL(u) is unknown
        {
            auto start = std::chrono::high_resolution_clock::now();
            u_prime = u->parent;
            v_prime = u_prime->SL;
            if (u_prime != root) // CASE IIA = SL(u) is unknown and u' is not root
            {
                u->SL = nodeHops(v_prime, s.substr(u->str.first, u->str.second), s, i);
                newLeaf = findPath(u->SL, s, i + u->SL->strDepth);
                auto end = std::chrono::high_resolution_clock::now();
                IIA_dur += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                IIA_executions++;
            }
            else { // Case IIB - SL(u) is unknown and u' is root
                beta_prime = s.substr(u->str.first + 1, u->str.second - 1);
                u->SL = nodeHops(root, beta_prime, s, i);
                newLeaf = findPath(u->SL, s, i + u->SL->strDepth); // findPath(v, s[1+|Beta_prime|...n])
                auto end = std::chrono::high_resolution_clock::now();
                IIB_dur += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                IIB_executions++;
            }
        }
		
        if (newLeaf->parent->strDepth > maxStrDepth)
        {
            *deepestIntNode = newLeaf->parent;
			maxStrDepth = newLeaf->parent->strDepth;
        }
    }
    if (IIA_executions != 0)
        std::cout << "\nIIA avg = " << IIA_dur / IIA_executions << " ns\n";
    if (IIB_executions != 0)
        std::cout << "\nIIB avg = " << IIB_dur / IIB_executions << " ns\n\n";
    return root;
}


std::string getPathLabel(Node* v, std::string s)
{
    std::string path = "";
    
    // while we are not at the root
    while (v->parent != v)
    {
        path = s.substr(v->str.first, v->str.second) + path;
        v = v->parent;
    }
    return path;
}

// Can land in middle of edge or on node
// Returns an internal node with path beta
Node* nodeHops(Node* v, std::string beta, std::string s, int i)
{
    extern std::unordered_map<char, int> ALPHA_VALS;
    extern int numIntNodes, numComparisons, numNodeHops;
    Node* child = v->children[ALPHA_VALS[beta[0]]];
    numComparisons++;
    if (child)
    {
        numNodeHops++;
        // if we need to keep going
        if (child->str.second < beta.length())
        {
            return nodeHops(child, beta.substr(child->str.second), s, i + child->str.second);
        }
        else if (child->str.second == beta.length()) // if beta falls directly on child
        {
            return child;
        }
        else // break the edge in the middle and create a new node
        {
            // how many do child and beta have in common
            int j = 0;
            std::string childStr = s.substr(child->str.first, child->str.second);

            j = beta.length();

            Node* newIntNode = new Node(numIntNodes++, v, nullptr, child->str.first, j);
            std::memcpy(&newIntNode->children[ALPHA_VALS[childStr[j]]], &child, sizeof(Node*));
            newIntNode->children[ALPHA_VALS[childStr[j]]]->parent = newIntNode;
            newIntNode->children[ALPHA_VALS[childStr[j]]]->str = { child->str.first + j, childStr.length() - j };
            v->children[ALPHA_VALS[beta[0]]] = newIntNode;

            return newIntNode;
        }
    }
    else // this will only be true when beta is ""
    {
        return v;
    }
}

// returns a pointer to the inserted leaf on path s.substr(i)
Node* findPath(Node* u, std::string s, int i)
{
    extern std::unordered_map<char, int> ALPHA_VALS;
    extern int numIntNodes, numLeaves, numComparisons;
    std::string x = s.substr(i);
    if (u->children[ALPHA_VALS[x[0]]])
    {
        int j = 0;
        // if the whole path label is the same as part of x
        std::string edgeString = s.substr(u->children[ALPHA_VALS[x[0]]]->str.first, u->children[ALPHA_VALS[x[0]]]->str.second);

        if (x.substr(0, edgeString.length()) == edgeString)
            return findPath(u->children[ALPHA_VALS[x[0]]], s, i + edgeString.length());
			
		
        for (j = 1; j < edgeString.length(); ++j) // start at 1 since we know the first will match
        {
            numComparisons++;
            if (x[j] != edgeString[j])
                break;
        }

        // if the edge's entire string matches x
        if (j >= edgeString.length())
        {
            // move down one node and repeat
            return findPath(u->children[ALPHA_VALS[x[0]]], s, i + j);
        }
        else
        {
            // insert into middle
            Node* newIntNode = new Node(numIntNodes++, u, nullptr, u->children[ALPHA_VALS[x[0]]]->str.first, j);
            newIntNode->children[ALPHA_VALS[x[j]]] = new Node(numLeaves++, newIntNode, nullptr, i + j, x.length() - j);
            std::memcpy(&newIntNode->children[ALPHA_VALS[edgeString[j]]], &u->children[ALPHA_VALS[x[0]]], sizeof(Node*));

            newIntNode->children[ALPHA_VALS[edgeString[j]]]->parent = newIntNode;
            newIntNode->children[ALPHA_VALS[edgeString[j]]]->str = { u->children[ALPHA_VALS[x[0]]]->str.first + j, edgeString.length() - j };
            u->children[ALPHA_VALS[x[0]]] = newIntNode;

            // return new leaf
            return newIntNode->children[ALPHA_VALS[x[j]]];
        }

    }
    else // if u does not have a child on that index
    {
        u->children[ALPHA_VALS[x[0]]] = new Node(numLeaves++, u, nullptr, i, x.length());
        return u->children[ALPHA_VALS[x[0]]];
    }
}


std::string getBWT(Node* v, std::string s)
{
    if (v)
    {
        std::string bwt = "";
        if (v->SL == nullptr)
        {
            if (v->id == 0)
                bwt += s[s.length() - 1];
            else
                bwt += s[v->id - 1];
            return bwt += "\n";
        }
        else
        {
            for (int i = 0; i < ALPHA_LEN + 1; i++)
                bwt += getBWT(v->children[i], s);
            return bwt;
        }
    }
    return "";
}


// depth first search
void DFS(Node* n)
{
    if (n)
    {
        for (int i = 0; i < ALPHA_LEN + 1; i++)
            DFS(n->children[i]);
        std::cout << n->strDepth << " ";
    }
}
