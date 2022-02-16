#include "alignments.hpp"

std::vector<std::vector<DP_cell*>> table;
//std::vector<std::vector<std::unique_ptr<DP_cell>>> table;
std::string s1, s2, s1_name, s2_name;
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

    // Reverse returned traceback strings
    std::reverse(s1_comp.begin(), s1_comp.end());
    std::reverse(s2_comp.begin(), s2_comp.end());

    // open results.txt for writing
    std::ofstream outfile;
    outfile.open("results.txt");
    

    // print out input
    outfile << "INPUT:\n******\n\n";
    outfile << s1_name << "\n";
    for (int i = 1; i <= s1.length(); i++)
    {
        outfile << s1[i - 1];
        if (i % 60 == 0)
            outfile << std::endl;
    }
    outfile << "\n\n" << s2_name << "\n";
    for (int i = 1; i <= s2.length(); i++)
    {
        outfile << s2[i - 1];
        if (i % 60 == 0)
            outfile << std::endl;
    }

    // Output
    outfile << "\n\nOUTPUT:\n*********\n";
    outfile << "Scores: \tmatch = " << match << ", mismatch = " << mismatch << ", h = " << h << ", g = " << g << std::endl;
    outfile << "Sequence 1 = \"s1\", length = " << s1.length() << " characters\n";
    outfile << "Sequence 2 = \"s2\", length = " << s2.length() << " characters\n\n";

    // formatted printing
    for (int i = 0; i < s1_comp.length() / 60 + 1; ++i)
    {
        outfile << "s1  " << i * 60 - s1_gap_count + 1<< "   \t";

        for (int j = i * 60; j < (i + 1) * 60; ++j)
        {
            if (j < s1_comp.length())
            {
                if (s1_comp[j] == '-')
                    s1_gap_count++;
                outfile << s1_comp[j];
            }
            else
                break;
        }

        outfile << "  " << (i + 1) * 60 - s1_gap_count  << std::endl << "  \t\t\t";

        for (int j = i * 60; j < (i + 1) * 60; ++j)
        {
            if (j < s1_comp.length())
            {
                if (s1_comp[j] == s2_comp[j])
                {
                    outfile << "|";
                    total_matches++;
                }
                else if (s1_comp[j] == '-')
                {
                    outfile << " ";
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
                    outfile << " ";
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

        outfile << std::endl << "s2  " << i * 60 - s2_gap_count + 1 << "   \t";


        for (int j = i * 60; j < (i + 1) * 60; ++j)
        {
            if (j < s2_comp.length())
            {
                if (s2_comp[j] == '-')
                    s2_gap_count++;
                outfile << s2_comp[j];
            }
            else
                break;
        }
        outfile << "  " << (i + 1) * 60 - s2_gap_count << std::endl << std::endl;

    }

    outfile << "Report: \n\n";
    outfile << ((local) ? "Local" : "Global") << " optimal score: " << score << "\n\n";
    outfile << "Number of: matches = " << total_matches << ", mismatches = " << total_mismatches << ", opening gaps = " << opening_gaps << ", gap extensions = " << gap_extensions << std::endl;
    outfile << "Identities = " << total_matches << "/" << s1_comp.length() << " (" <<  (int)floor((double)total_matches * 100 + 0.5) / s1_comp.length() << "%)" << ", Gaps = " << gap_extensions << "/" << s1_comp.length() << " (" << (int)floor((double)gap_extensions * 100 + 0.5) / s1_comp.length() << "%)" << std::endl;

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