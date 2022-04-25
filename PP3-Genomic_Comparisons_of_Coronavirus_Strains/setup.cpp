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
	// comment/uncomment to turn on/off automatic file selection
	//files.resize(10);
	//files = {
	//	"Covid_Australia.fasta",
	//	"Covid_Brazil.fasta",
	//	"Covid_India.fasta",
	//	"Covid_USA-CA4.fasta",
	//	"Covid_Wuhan.fasta",
	//	"MERS_2012_KF600620.fasta",
	//	"MERS_2014_KY581694.fasta",
	//	"MERS_2014_USA_KP223131.fasta",
	//	"SARS_2003_GU553363.fasta",
	//	"SARS_2017_MK062179.fasta"
	//};
	files.resize(5);
	files = {
		"s1.txt",
		"s2.txt",
		"s3.txt",
		"s4.txt",
		"s5.txt"
	};

	int i = 0;
	std::string line = "";
	for (auto file : files)
	{
		std::ifstream infile(file);
		if (infile.is_open())
		{
			std::getline(infile, line); // Throw out garbage line
			contents.push_back(""); // this is done here instead of at the start to account for the possibility of files not opening
			while (std::getline(infile, line))
			{
				contents.at(i) += line;
			}
			contents.at(i) += "$";
			i++;
		}
		else
			std::cout << "Failed to open file " << file << "... Skipping\n";
	}
	k = contents.size();
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
	ALPHA_VALS.insert(std::pair<char, int>('$', 0));

	std::string line = "";
	std::getline(infile, line);

	for (char c : line)
	{
		// ignore space characters
		if (c != ' ')
			ALPHA_VALS.insert(std::pair<char, int>(c, ALPHA_VALS.size()));
	}

	return ALPHA_VALS;
}

void set_contents(std::vector<std::string> c)
{
	contents.resize(c.size());
	for (int i = 0; i < c.size(); ++i)
	{
		contents[i] = c[i];
	}
}