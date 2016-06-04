// Main executable file for the Lexical Analyzer

#include <iostream>
#include <fstream>
#include <memory>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "exception.h"
#include "PrintVisitor.h"
#include "VariableVisitor.h"

// Class that holds all the options for how the program is run
class Options
{
public:
  // Constructor
  Options() :
    parse(false),
    print(true)
  {}

  // Sets the "parse only" flag (-p)
  void set_parse_only(bool p)
    { parse = p; }

  // Gets the "parse only" flag
  bool parse_only()
    { return parse; }

  // Sets the "print" flag (-p)
  void set_print(bool p)
    { print = p; }

  // Gets the "print" flag
  bool get_print()
    { return print; }

private:
  // The "parse only" flag
  bool parse;

  // THe print flag
  bool print;
};

void printAST(std::ostream& out, std::shared_ptr<StmtList> ast, std::string filename)
{
  // Create the PrintVisitor used to print out all the ASTs
  PrintVisitor vtor(out);

  // Print out the filename and the AST
  out << "AST of " << filename << ":" << std::endl;
  ast->accept(vtor);
  out << std::endl << std::endl;
}

void variableAST(std::ostream& out, std::shared_ptr<StmtList> ast, std::string filename)
{
  // Create the VariableVisitor
  VariableVisitor vtor = VariableVisitor(out);

  // Pass the visitor to the AST
  ast->accept(vtor);

  // What did the VariableVisitor find out?
  out << "Variable knowledge of " << filename << ":" << std::endl;
  vtor.print_knowledge();
  out << std::endl << std::endl;
}

// Runs the meat and potatoes of the program
// Parses every file passed in in the files parameter
void run(Options& opt, std::ostream& out, std::deque<std::string>& files)
{
  // Loop through all passed in files
	for (std::string filename: files)
  {
    // Attempt to open the file
    std::ifstream file(filename);
    if(!file.is_open())
    {
      std::cerr << "ERROR: Unable to open '" << filename << "'" << std::endl;
      continue;
    }

    // Parses the file then prints the resulting AST
    try
    {
      // Start the lexer
      Lexer lexer(file);

      // Start the parser,
      // parse the file
      std::shared_ptr<StmtList> ast = Parser(lexer).parse();

      // Stop here if parsing was all that was specified
      if (opt.parse_only())
        continue;

      // Print out the filename and the AST
      if (opt.get_print())
        printAST(out, ast, filename);

      // Catch variable errors
      variableAST(out, ast, filename);
    }
    catch(Exception e)
    {

      std::cerr << "In file " << filename << ":" << std::endl
          << e.what() << std::endl;
    }

    // Close the stream
    file.close();
  }
}

// The main function for this program.
// Checks for proper arguments, exits if they are incorrect.
int main(int argc, char *argv[])
{
  // The options that determine how the program runs
  Options opt = Options();

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
        std::cerr << "Argument error. No filename specified after the -o flag. "
                  << "Defaulting to std::cout." << std::endl;
    }
    else if (arg.compare("-p") == 0)
    {
      // Only parse the files, checking for syntactical correctness
      opt.set_parse_only(true);
    }
    else if (arg.compare("-no-print") == 0)
    {
      // Don't print out the ASTs
      opt.set_print(false);
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

  // Check that there are files specified
	if (files.empty())
	{
		std::cerr << "USAGE: " << argv[0] << " [-o output_filename] file [file] [file] [...]" << std::endl;
		return -1;
	}

  // Run the program
  run(opt, outFile, files);

	return 0;
}
