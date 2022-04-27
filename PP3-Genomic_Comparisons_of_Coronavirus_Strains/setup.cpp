#include "setup.hpp"

std::vector<std::string> get_input_files()
{
	int num_files = -1;
	do {
		std::cout << "How many input files? : ";
		std::cin >> num_files;
	} while (num_files < 1);

	std::vector<std::string> files;
	for (int i = 0; i < num_files; i++) {
		std::string file;
		std::cout << "Enter file name (" << i + 1 << "): ";
		std::cin >> file;
		files.push_back(file);
	}

	return files;
}


void read_input_files()
{
	std::vector<std::string> files;
	// comment/uncomment to turn on/off automatic file selection
	std::cout << "Please make a selection:\n1. Default DNA files\n2. Default English Files\n3. Custom inputs\n";
	int selection = -1;
	do {
		std::cout << "Selection: ";
		std::cin >> selection;
	} while (selection == -1);


	switch (selection) {
		case 1:
			files.resize(10);
			files = {
				".\\Sequences\\Covid_Australia.fasta", // 0
				".\\Sequences\\Covid_Brazil.fasta", // 1		
				".\\Sequences\\Covid_India.fasta", // 2
				".\\Sequences\\Covid_USA-CA4.fasta", // 3
				".\\Sequences\\Covid_Wuhan.fasta", // 4
				".\\Sequences\\MERS_2012_KF600620.fasta", // 5
				".\\Sequences\\MERS_2014_KY581694.fasta", // 6
				".\\Sequences\\MERS_2014_USA_KP223131.fasta", // 7
				".\\Sequences\\SARS_2003_GU553363.fasta", // 8
				".\\Sequences\\SARS_2017_MK062179.fasta" // 9
			}; 
		break;
		case 2:
			files.resize(5);
			files = {
				".\\Sequences\\s1.txt", // 0
				".\\Sequences\\s2.txt", // 1
				".\\Sequences\\s3.txt", // 2
				".\\Sequences\\s4.txt", // 3
				".\\Sequences\\s5.txt"  // 4
			};
		break;
		default: // If 3 or a non-existent option is selected
			files = get_input_files();
		break;
	}

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
}

std::string get_alphabet_file()
{
	std::string name;
	std::cout << "Enter the name of the alphabet file: ";
	std::cin >> name;
	return name;
}


void read_alphabet_file()
{
	int a_sel = -1;
	std::string a_file = "";
	std::cout << "\nPlease make a selection:\n1. DNA Alphabet\n2. English Alphabet\n3. Custom alphabet\n";
	do {
		std::cout << "Selection: ";
		std::cin >> a_sel;
	} while (a_sel == -1);

	switch (a_sel)
	{
		case 1:
			a_file = ".\\Alphabets\\DNA_alphabet.txt";
		break;
		case 2:
			a_file = ".\\Alphabets\\English_alphabet.txt";
		break;
		default: // If 3 or a non-existent option is selected
			a_file = get_alphabet_file();
		break;
	}


	std::ifstream infile(a_file);
	while (!infile.is_open())
	{
		std::cout << "Failed to open alphabet file. It's kind of important so please enter another name.\n\n";
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
}
