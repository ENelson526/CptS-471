#include "alignments.hpp"

// reads the sequence data from the file provided through command line arguments
void read_sequence_file(std::string filename)
{
    extern std::string s1, s2, s1_name, s2_name;
    std::ifstream infile;
    std::string line;
    infile.open(filename);

    if (infile.is_open())
    {   
        // read name line s1
        getline(infile, line);
        s1_name = line;

        // read in sequence1
        getline(infile, line);
        while (line.length() != 0)
        {
            s1 += line;
            getline(infile, line);
        }

        // read name line for s2
        getline(infile, line); 
        s2_name = line;

        // read in sequence2
        getline(infile, line);
        while (!infile.eof())
        {
            s2 += line;
            std::cout << line << std::endl;
            getline(infile, line);
        }

        // close infile
        infile.close();
    }
}


// reads the config file given through the command line arguments (optional)
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


// finds and returns the max value given three integers
int find_max(int i, int s, int d)
{
    if (i > std::max(s, d))
        return i;
    else if (s > d)
        return s;
    return d;
}


// fills the data table for the global sequence alignment
void fill_global_table()
{
   // extern std::vector<std::vector<std::unique_ptr<DP_cell>>> table;
    extern std::vector<std::vector<DP_cell*>> table;
    extern std::string s1, s2;
    extern int g, match, mismatch, h;

    // init table size
    table.resize(s1.length() + 1);
    for (int i = 0; i < s1.length() + 1; ++i)
        table[i].resize(s2.length() + 1);
    std::cout << "s1_table_len : " << table.size() << std::endl;
    std::cout << "s2_table_len : " << table[0].size() << std::endl;
    int t = 0;

    for (int i = 0; i < s1.length() + 1; ++i)
    {
        for (int j = 0; j < s2.length() + 1; ++j)
        {
            t++;
            if (t % 1000000 == 0)
            {
                std::cout << t << std::endl;
            }
            try
            {
                table[i][j] = new DP_cell();
            }
            catch (std::bad_alloc)
            {
                std::cout << "Cannot allocate space for new DP_cell. Aborting\n";
                abort();
            }
        }
    }

    for (int i = 0; i < table.size(); ++i)
    {
        table[i][0]->value = i * g;
        table[i][0]->direction = up;
        table[i][0]->Sscore = INT_MIN - h - g;
        table[i][0]->Dscore = h + (i * g);
        table[i][0]->Iscore = INT_MIN - h - g;
    }
    for (int j = 0; j < table[0].size(); ++j)
    {
        table[0][j]->value = j * g;
        table[0][j]->direction = left;
        table[0][j]->Sscore = INT_MIN - h - g; // without this, the addition of + h + g will cause an overflow
        table[0][j]->Dscore = INT_MIN - h - g;
        table[0][j]->Iscore = h + (j * g);
    }

    // loop through rows
    for (int i = 1; i <= s1.length(); ++i)
    {
        // loop through columns
        for (int j = 1; j <= s2.length(); ++j)
        {
            // Find max values
            table[i][j]->Sscore = find_max(table[i-1][j-1]->Iscore, table[i-1][j-1]->Sscore, table[i-1][j-1]->Dscore) + ((s1[i-1] == s2[j-1]) ? match : mismatch);
            table[i][j]->Dscore = find_max(table[i-1][j]->Dscore + g, table[i-1][j]->Sscore + h + g, table[i-1][j]->Iscore + h + g);
            table[i][j]->Iscore = find_max(table[i][j-1]->Iscore + g, table[i][j-1]->Sscore + h + g, table[i][j-1]->Dscore + h + g);

            // Find which was to proceed to maximize score
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


// finds the optimal global alignment of the given sequences. Requires fill_global_table() to be called first
int optimal_global(std::string* s1_comp, std::string* s2_comp)
{
    extern std::vector<std::vector<DP_cell*>> table;
    extern std::string s1, s2;
    extern int g, match, mismatch, total_matches, total_mismatches;
    int i = table.size() - 1, j = table[0].size() - 1;
    std::cout << i << " " << j << "\n";
    int score = table[i][j]->value;
    //std::cout << "i = " << i << ", j = " << j << std::endl;
    DP_cell* current = table[i][j];
    
    while (current != table[0].at(0) && s1_comp)
    {
        if (j < 0)
            current = table[i].at(0);
        else if (i < 0)
            current = table[0].at(j);
        else
            current = table[i][j];
        if (current == table[0].at(0))
            break;

        switch (current->direction)
        {
            case left:
                --j;
                *s1_comp += "-";
                *s2_comp += (j >= 0) ? s2[j] : '-';
                break;
            case diagonal:
                i--; j--;
                *s1_comp += (i >= 0) ? s1[i] : '-';
                *s2_comp += (j >= 0) ? s2[j] : '-';
                break;
            case up:
                --i;
                *s1_comp += (i >= 0) ? s1[i] : '-';
                *s2_comp += "-";
                break;
        }
    }

    return score;
}

int find_max_or_zero(int v1, int v2, int v3)
{
    if (v1 < 0 && v2 < 0 && v3 < 0)
        return 0;
    else
        return find_max(v1, v2, v3);
}


void fill_local_table()
{
    extern std::vector<std::vector<DP_cell*>> table;
    extern std::string s1, s2;
    extern int match, mismatch, g, h;

    table.resize(s1.length() + 1);
    for (int i = 0; i < s1.length() + 1; ++i)
        table[i].resize(s2.length() + 1);
    
    std::cout << "s1_table_len : " << table.size() << std::endl;
    std::cout << "s2_table_len : " << table[0].size() << std::endl;
    int t = 0;
    for (int i = 0; i <= s1.length(); ++i)
    {
        for (int j = 0; j <= s2.length(); ++j)
        {
            t++;
            if (t % 1000000 == 0)
                std::cout << t << std::endl;
            try 
            {
                table[i][j] = new DP_cell();
            }
            catch(std::bad_alloc)
            {
                std::cout << "Cannot allocate space for new DP_cell. Aborting\n";
                abort();
            }
        }
    }

    for (int i = 0; i < table.size(); ++i)
    {
        table[i][0]->value = 0;
        table[i][0]->direction = up;
        table[i][0]->Sscore = INT_MIN - h - g;
        table[i][0]->Dscore = h + (i * g);
        table[i][0]->Iscore = INT_MIN - h - g;
    }
    for (int j = 0; j < table[0].size(); ++j)
    {
        table[0][j]->value = 0;
        table[0][j]->direction = left;
        table[0][j]->Sscore = INT_MIN - h - g; // without this, the addition of + h + g will cause an overflow
        table[0][j]->Dscore = INT_MIN - h - g;
        table[0][j]->Iscore = h + (j * g);
    }

    for (int i = 1; i <= s1.length(); ++i)
    {
        // loop through columns
        for (int j = 1; j <= s2.length(); ++j)
        {
            // Find max values
            table[i][j]->Sscore = find_max_or_zero(table[i-1][j-1]->Iscore, table[i-1][j-1]->Sscore, table[i-1][j-1]->Dscore) + ((s1[i-1] == s2[j-1]) ? match : mismatch);
            table[i][j]->Dscore = find_max_or_zero(table[i-1][j]->Dscore + g, table[i-1][j]->Sscore + h + g, table[i-1][j]->Iscore + h + g);
            table[i][j]->Iscore = find_max_or_zero(table[i][j-1]->Iscore + g, table[i][j-1]->Sscore + h + g, table[i][j-1]->Dscore + h + g);

             // Find which was to proceed to maximize score
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



int local_alignment(std::string* s1_comp, std::string* s2_comp)
{
    std::cout << "trace back\n";
    extern std::vector<std::vector<DP_cell*>> table;
    extern std::string s1, s2;
    extern int match, mismatch, g, h;
    int row, col, max_so_far = 0;

    for (int i = 0; i < table.size(); ++i)
    {
        for (int j = 0; j < table[0].size(); ++j)
        {
            if (table[i][j]->value > max_so_far)
            {
                max_so_far = table[i][j]->value;
                row = i; col = j;
            }        
        }
    }

    std::cout << "starting coord: " << row << " " << col << std::endl;
    
    // if there is no alignment
    if (max_so_far == 0)
    {
        std::cout << "No local_alignment\n";
        return 0;
    }

    // now that we have the max index, trace back
    DP_cell* current = table[row][col];
    while (current->value > 0)
    {
        switch (current->direction)
        {
        case left:
            --col;
            *s1_comp += "-";
            *s2_comp += (col >= 0) ? s2[col] : '-';
            break;
        case diagonal:
            --row; --col;
            *s1_comp += (row >= 0) ? s1[row] : '-';
            *s2_comp += (col >= 0) ? s2[col] : '-';
            break;
        case up:
            --row;
            *s1_comp += (row >= 0) ? s1[row] : '-';
            *s2_comp += "-";
            break;
        }
        
        current = table[row][col];
    }

    std::cout << "ending coord: " << row << " " << col << std::endl;

    return max_so_far;
}