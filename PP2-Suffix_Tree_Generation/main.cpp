#include "tree.hpp"
std::unordered_map<char, int> ALPHA_VALS = {};
int numIntNodes = 0, numLeaves = 0, numEdges = 0, numComparisons = 0, numNodeHops = 0;
int main(int argc, char* argv[])
{
    if (argc >= 4)
    {
        Node* deepestInternalNode = nullptr;
        std::string s = readSequenceFile(argv[1]);
        readAlphabetFile(argv[2]);
        auto start = std::chrono::high_resolution_clock::now();
        Node* r = build_ST(s, &deepestInternalNode);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Total runtime in ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
        std::cout << "numLeaves = " << numLeaves << "\nnumIntNodes = " << numIntNodes << "\ntotalNodes = " << numLeaves + numIntNodes << "\n";
        std::cout << "numComparisons: " << numComparisons << ", numNodeHops: " << numNodeHops << std::endl;

        std::cout << "Exact Matching Repeat: " << getPathLabel(deepestInternalNode, s) << " at indexes ";

        // find starting indexes of this repeat
        for (auto n : deepestInternalNode->children)
        {
            if (n)
                std::cout << n->id << " ";
        }
        std::cout << "\n\n";
        
        std::cout << "Calculating and writing BWT to file...\n";
        // BWT output
        std::ofstream outfile;
        outfile.open(argv[3]);
        outfile << getBWT(r, s + "$");
        outfile.close();
		std::cout << "DONE!\n";
        
        std::cout << "\nWould you like to see the tree's DFS strDepths? (y/n): ";
        char c;
        std::cin >> c;
        if (c == 'y')
        {
            std::cout << "\n\n";
            DFS(r);
        }
    }
    else
        std::cout << "Please enter the input filename, the input alphabet name, and the desired output filename as command line arguments.\n";

    return 0;
}
