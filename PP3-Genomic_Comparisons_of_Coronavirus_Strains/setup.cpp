#include "setup.hpp"

std::vector<std::string> get_input_files()
{
	int num_files = -1;
	do {
		std::cout << "How many input files? : ";
		std::cin >> num_files;
	} while (num_files == -1);

	if (num_files == 0) {
		std::cout << "Awesome! I don't have to do anything!\n";
		abort();
	}

	// If we have to do things :(
	std::vector<std::string> files;
	for (int i = 0; i < num_files; i++) {
		std::string file;
		std::cout << "Enter file name (" << i + 1 << "): ";
		std::cin >> file;
		files.push_back(file);
	}

	return files;
}


std::vector<std::string> read_input_files(std::vector<std::string> files)
{
	std::vector<std::string> contents;
	int i = 0;
	std::string line = "";
	for (auto file : files)
	{
		std::ifstream infile(file);
		if (infile.is_open())
		{
			contents.push_back(""); // this is done here instead of at the start to account for the possibility of files not opening
			while (std::getline(infile, line))
			{
				contents.at(i) += line;
			}
			i++;
		}
		else
			std::cout << "Failed to open file " << file << "... Skipping\n";
	}

	return contents;
}

std::string get_alphabet_file()
{
	std::string name;
	std::cout << "Enter the name of the alphabet file: ";
	std::cin >> name;
	return name;
}


std::unordered_map<char, int> read_alphabet_file(std::string a_file)
{
	std::ifstream infile(a_file);
	while (!infile.is_open())
	{
		std::cout << "Failed to open alphabet file. It's kind of important so please try again.\n\n";
		infile.open(get_alphabet_file());
	}

	// infile should be open
	std::unordered_map<char, int> alphabet;
	alphabet.insert(std::pair<char, int>('$', 0));

	std::string line = "";
	std::getline(infile, line);

	for (char c : line)
	{
		// ignore space characters
		if (c != ' ')
			alphabet.insert(std::pair<char, int>(c, alphabet.size()));
	}

	return alphabet;
}