// Main executable file for the Lexical Analyzer

#include <iostream>
#include <fstream>
#include <memory>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "exception.h"

// Runs the meat and potatoes of the program
// Parses every file passed in in the files parameter
void run(std::ostream& out, std::deque<std::string>& files)
{
  // Loop through all passed in files
	for (std::string filename: files)
  {
    // Attempt to open the file
    std::ifstream file(filename);
    if(!file.is_open())
    {
      std::cout << "ERROR: Unable to open '" << filename << "'" << std::endl;
      return;
    }

    // Parses the file then prints the resulting AST
    try
    {
      // Start the lexer
      Lexer lexer(file);

      // Start the parser,
      // parse the file
      std::shared_ptr<StmtList> ast = Parser(lexer).parse();

      // Print out the filename and the AST
      out << "AST of " << filename << ":" << std::endl;
      ast->print(out, " ");
      out << std::endl << std::endl;
    }
    catch(Exception e)
    {
      std::cout << e.what() << std::endl;
    }

    // Close the stream
    file.close();
  }
}

// The main function for this program.
// Checks for proper arguments, exits if they are incorrect.
int main(int argc, char *argv[])
{
	// Check the proper number of arguments
	if(argc < 2)
	{
		std::cout << "USAGE: " << argv[0] << " [-o output_filename] file [file] [file] [...]" << std::endl;
		return -1;
	}

	// The files to parse
	std::deque<std::string> files;

	// Will be set to the output filename if one is specified
	std::string output_file;

	// Filter the arguments
	for (int i = 1; i < argc; i++)
  {
    std::string arg = argv[i];
    if (arg.compare("-o") == 0)
    {
      if (++i < argc)
        // Set the file to output to
        output_file = argv[i];
      else
        // Error. -o was the last argument, there is no specified file
        std::cout << "Argument error. No filename specified after the -o flag. "
                  << "Defaulting to std::cout." << std::endl;
    }
    else
      // Add the file to the parse list
      files.push_back(arg);
  }

  // Declaration of the output file stream
  std::ofstream out_stream;

  // Open the output file stream if an output file was specified
  if (output_file.size() > 0)
    out_stream.open(output_file, std::ios::out);

  // Where to post the output
  std::ostream& outFile = (output_file.size() > 0 ? out_stream : std::cout);

  // Run the program
  run(outFile, files);

	return 0;
}
