#include "alignments.hpp"

std::vector<std::vector<DP_cell*>> table;
//std::vector<std::vector<std::unique_ptr<DP_cell>>> table;
std::string s1, s2;
int match = 1, mismatch = -1, h = 0, g = -2;
int total_matches, total_mismatches, opening_gaps, gap_extensions;

// expecting <executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>
int main(int argc, char* argv[])
{
    std::string s1_comp, s2_comp;
    int score = 0, s1_gap_count = 0, s2_gap_count = 0;
    bool local = false;
    if (argc >= 3)
    {
        read_sequence_file(argv[1]);
        local = std::stoi(argv[2]);

        // if a config file name was provided
        if (argc >= 4)
            read_config_file(argv[3]);
    }

    std::cout << "s1 len = " << s1.length() << std::endl;
    std::cout << "s2 len = " << s2.length() << std::endl;
    
    if (local)
    {
        fill_local_table();
        score = local_alignment(&s1_comp, &s2_comp);
    }
    else
    {
        fill_global_table();
        score = optimal_global(&s1_comp, &s2_comp);
    }

    // Reverse returned strings
    std::reverse(s1_comp.begin(), s1_comp.end());
    std::reverse(s2_comp.begin(), s2_comp.end());

    
    // Output
    std::cout << "OUTPUT:\n*********\n";
    std::cout << "Scores: \tmatch = " << match << ", mismatch = " << mismatch << ", h = " << h << ", g = " << g << std::endl;
    std::cout << "Sequence 1 = \"s1\", length = " << s1.length() << " characters\n";
    std::cout << "Sequence 2 = \"s2\", length = " << s2.length() << " characters\n";
    
    std::cout << "\n\nAlignment Start\n*****************\n";
    

    // formatted printing
    for (int i = 0; i < s1_comp.length() / 60 + 1; ++i)
    {
        std::cout << "s1  " << i * 60 - s1_gap_count + 1<< "   \t";

        for (int j = i * 60; j < (i + 1) * 60; ++j)
        {
            if (j < s1_comp.length())
            {
                if (s1_comp[j] == '-')
                    s1_gap_count++;
                std::cout << s1_comp[j];
            }
            else
                break;
        }

        std::cout << "  " << (i + 1) * 60 - s1_gap_count  << std::endl << "  \t\t";

        for (int j = i * 60; j < (i + 1) * 60; ++j)
        {
            if (j < s1_comp.length())
            {
                if (s1_comp[j] == s2_comp[j])
                {
                    std::cout << "|";
                    total_matches++;
                }
                else if (s1_comp[j] == '-')
                {
                    std::cout << " ";
                    try 
                    {
                        if (s1_comp.at(j - 1) != '-')
                            opening_gaps++;
                    }
                    catch (std::out_of_range)
                    {
                        opening_gaps++;
                    }
                    gap_extensions++;
                }
                else
                {
                    std::cout << " ";
                    try 
                    {
                        if (s2_comp.at(j - 1) != '-')
                            opening_gaps++;
                    }
                    catch (std::out_of_range)
                    {
                        opening_gaps++;
                    }   
                    gap_extensions++;
                }
            }
        }

        std::cout << std::endl << "s2  " << i * 60 - s2_gap_count + 1 << "   \t";


        for (int j = i * 60; j < (i + 1) * 60; ++j)
        {
            if (j < s2_comp.length())
            {
                if (s2_comp[j] == '-')
                    s2_gap_count++;
                std::cout << s2_comp[j];
            }
            else
                break;
        }
        std::cout << "  " << (i + 1) * 60 - s2_gap_count << std::endl << std::endl;

    }


    std::cout << "Score: " << score << std::endl;
    std::cout << "Number of: matches = " << total_matches << ", mismatches = " << total_mismatches << ", opening gaps = " << opening_gaps << ", gap extensions = " << gap_extensions << std::endl;
    std::cout << "Identities = " << total_matches << "/" << s1_comp.length() << " (" <<  (int)floor((double)total_matches * 100 + 0.5) / s1_comp.length() << "%)" << ", Gaps = " << gap_extensions << "/" << s1_comp.length() << " (" << (int)floor((double)gap_extensions * 100 + 0.5) / s1_comp.length() << "%)" << std::endl;

    // free memory
    for (std::vector<DP_cell*> v : table)
    {
        for (DP_cell* dp : v)
        {
            free(dp);
        }
    }
   

    return 0;
}