#include "alignments.hpp"

std::vector<std::vector<DP_cell*>> table;
std::string s1, s2;
int match, mismatch, h, g;

// expecting <executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>
int main(int argc, char* argv[])
{
    std::string s1_comp, s2_comp;
    int score = 0;
    bool local = false;
    if (argc >= 3)
    {
        read_sequence_file(argv[1]);
        local = std::stoi(argv[2]);

        // if a config file name was provided
        if (argc >= 4)
            read_config_file(argv[3]);
    }

    if (local)
        local_alignment();
    else
    {
        fill_global_table();
        score =optimal_global(&s1_comp, &s2_comp);
    }

    /*
    // Output
    std::cout << "OUTPUT:\n*********\n";
    std::cout << "Scores: \tmatch = " << match << ", mismatch = " << mismatch << ", h = " << h << ", g = " << g << std::endl;
    std::cout << "Sequence 1 = \"s1\", length = " << s1.length() << " characters\n";
    std::cout << "Sequence 2 = \"s2\", length = " << s2.length() << " characters\n";


    std::cout << "s1  1    " << s1_comp << std::endl;
    std::cout << "s2  1    " << s2_comp << std::endl;

    std::cout << "Score: " << score << std::endl;
    */

   std::cout << "s1 = " << s1 << "\ns2 = " << s2 << std::endl;

   std::cout << "    "; 
   for (int i = 0; i < s1.length(); ++i)
   {
       std::cout << s1[i] << "  ";
   }
   std::cout << std::endl << "  ";
   for (int i = 0; i < s1.length() + 1; ++i)
   {
       for (int j = 0; j < s2.length() + 1; ++j)
       {
           std::cout << table[i][j]->value << " ";
       }
       std::cout << std::endl << s2[i] << " ";
   }


    return 0;
}