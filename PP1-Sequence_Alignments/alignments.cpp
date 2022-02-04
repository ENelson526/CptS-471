#include "alignments.hpp"

void read_sequence_file(std::string filename)
{
    extern std::string s1, s2;
    std::ifstream infile;
    infile.open(filename);

    if (infile.is_open())
    {
        // get s1
        getline(infile, s1);

        // get s2
        getline(infile, s2);

        // close infile
        infile.close();
    }
}

void read_config_file(std::string filename)
{
    extern int match, mismatch, h, g;
    std::string line, delimeter = " ", token = "";
    size_t pos = 0;
    std::ifstream infile;
    infile.open(filename);
    
    if (infile.is_open())
    {
        for (int i = 0; i < 4; ++i)
        {
            getline(infile, line);
            token = line.substr(line.find(delimeter) + 1, line.length() - 1);
            switch(i)
            {
                case 0: // match
                    match = std::stoi(token);
                    break;
                case 1: // mismatch
                    mismatch = std::stoi(token);
                    break;
                case 2: // h
                    h = std::stoi(token);
                    break;
                case 3: // g
                    g = std::stoi(token);
                    break;
                default:
                    break;
            }
        }
        infile.close();
    }
}


void fill_global_table()
{
    extern std::vector<std::vector<DP_cell*>> table;
    extern std::string s1, s2;
    extern int g, match, mismatch;

    // init table values
    table.resize(s1.length() + 1);
    for (int i = 0; i < s1.length() + 1; ++i)
        table[i].resize(s2.length() + 1);
    

    // init 0 row and column
    for (int i = 0; i < s1.length() + 1; ++i)
    {
        for (int j = 0; j < s2.length() + 1; ++j)
        {
            table[i][j] = new DP_cell();
            if (i == 0)
            {
                table[i][j]->value = j * g;
                table[i][j]->direction = up;
            }
            if (j == 0)
            {
                table[i][j]->value = i * g;
                table[i][j]->direction = left;
            }
        }
    }

    // loop through rows
    for (int i = 1; i <= s1.length(); ++i)
    {
        // loop through columns
        for (int j = 1; j <= s2.length(); ++j)
        {
            // T(i-1, j-1) + S(ai, bj)
            table[i][j]->Sscore = table[i-1][j-1]->value + ((s1[i] == s2[j]) ? match : mismatch);

            // T(i-1, j) + g
            table[i][j]->Dscore = table[i-1][j]->value + g;
            
            // T(i, j-1) + g
            table[i][j]->Iscore = table[i][j-1]->value + g;

            
            if (table[i][j]->Sscore > std::max(table[i][j]->Dscore, table[i][j]->Iscore)) // Substitution (S)
            {
                table[i][j]->direction = diagonal;
                table[i][j]->value = table[i][j]->Sscore;
            }
            else if (table[i][j]->Dscore > table[i][j]->Iscore) // Deletion (D)
            {
                table[i][j]->direction = up;
                table[i][j]->value = table[i][j]->Dscore;
            }
            else    // Insertion (I)
            {
                table[i][j]->direction = left;
                table[i][j]->value = table[i][j]->Iscore;
            }
        }
    }
}

int optimal_global(std::string* s1_comp, std::string* s2_comp)
{
    extern std::vector<std::vector<DP_cell*>> table;
    extern std::string s1, s2;
    int y = table.size() - 1, x = table[0].size() - 1;
    int score = table[y][x]->value;
    std::cout << "y = " << y << ", x = " << x << std::endl;

    while (table[y][x] != table[0][0])
    {
        std::cout << "(x,y) = (" << x << ", " << y << "), Value = " << table[y][x]->value << ", Direction = " << table[y][x]->direction << ", s1 = " << s1[y] << ", s2 = " << s2[x] << std::endl;
        switch(table[y][x]->direction)
        {
            case up:
                *s1_comp += s1[y];
                *s2_comp += '-';
                y--;
                score += table[y][x]->value;
                break;
            case diagonal:
                *s1_comp += s1[y];
                *s2_comp += s2[x];
                y--;
                x--;
                score += table[y][x]->value;
                break;
            case left:
                *s1_comp += '-';
                *s2_comp += s2[x];
                x--;
                score += table[y][x]->value;
                break;
        }
    }


    /*
    // if up diagonal left == max
    while (x != 0 && y != 0)
    {
        if (table[y-1][x-1]->value > std::max(table[y-1][x]->value, table[y][x-1]->value))
        {
            *s1_comp += s1[y-1];
            *s2_comp += s2[x-1];
            y--; x--;
        }
        else if (table[y][x-1]->value > table[y-1][x]->value) // if left == max
        {
            *s1_comp += '-';
            *s2_comp += s2[x-1];
            x--;
        }
        else // if up == max
        {
            *s1_comp += s1[y-1];
            *s2_comp += '-';
            y--;
        }
        score += table[y][x]->value;
    }
    */

    (*s1_comp).reserve();
    (*s2_comp).reserve();

    return score;
}



int local_alignment()
{
    return 0;
}