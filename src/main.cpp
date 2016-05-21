// Main executable file for the Lexical Analyzer

#include <iostream>
#include <fstream>
#include <memory>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "exception.h"

// The main function for this program.
// Checks for proper arguments, exits if they are incorrect.
int main(int argc, char *argv[])
{
	// Check the proper number of arguments
	if(argc < 2)
	{
		std::cout << "USAGE: " << argv[0] << " file" << std::endl;
		return -1;
	}

	// Attempt to open the file
	std::ifstream file(argv[1]);
	if(!file.is_open())
	{
		std::cout << "ERROR: Unable to open '" << argv[1] << "'" << std::endl;
		return -1;
	}

	// Parses the file, only prints if an error is encountered
	try
	{
		// Start the lexer
		Lexer lexer(file);
		// Start the parser
		Parser parser(lexer);
		// Parse the stuff and print the tree
		parser.parse()->print(std::cout, "");
	}
	catch(Exception e)
	{
		std::cout << e.what() << std::endl;
	}

	// Close the stream
	file.close();
}
