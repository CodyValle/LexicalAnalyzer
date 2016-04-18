// Main executable file for the Lexical Analyzer

#include <iostream>
#include <fstream>
#include <memory>

#include "token.h"
#include "lexer.h"
#include "exception.h"

// The main function for this program.
// Checks for proper arguments, exits if they are incorrect.
// Otherwise it prints out every token found in the passed in filename.
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

	// Start lexing the file
	try
	{
		Lexer lexer(file);
		Token t = lexer.next_token();
		std::cout << t << std::endl;
		while(t.get_type() != TokenType::EOS) // Until the end of file is reached
		{
			t = lexer.next_token();
			std::cout << t << std::endl;
		}
	}
	catch(Exception e)
	{
		std::cout << e.what() << std::endl;
	}

	// Close the stream
	file.close();
}
