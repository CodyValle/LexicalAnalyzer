#ifndef ASMSTRUCTURE_H_INCLUDED
#define ASMSTRUCTURE_H_INCLUDED

// Declares the AsmStructure class

#include <iostream>
#include <list>
#include <string>

class Procedure
{
public:
	// Constructor
	Procedure(std::string);

	// Add a string instruction
	void add_instruction(std::string in)
		{ instructions.push_back(in); }; // std::cerr << "Adding instruction: " << in << std::endl; std::cerr << "Last instruction: " << instructions.back() << std::endl;

	std::string get_name()
		{ return name; }

private:
	// Name of this procedure
	std::string name;

	// List of lines in .text section
	std::list<std::string> instructions;

	// To access private members without accessors
	friend std::ostream& operator<<(std::ostream&, const Procedure&);
};

/// A structure that contains all assembly data
class AsmStructure
{
public:
	// Constructor
	AsmStructure();

	// Converts the structure into assembly as puts it in the passed in stream
	void convert(std::ostream&);

	// Add a string constant to the program
	void add_constant(std::string, std::string);

	// Add an integer constant to the program
	void add_constant(std::string, int, bool);

	// Add a boolean constant to the program
	void add_constant(std::string, bool);

	// Add a variable to the program
	void add_variable(std::string, int);

	// Add a procedure to the program
	void add_procedure(Procedure&);

private:
	// List of lines in .data section
	std::list<std::string> constants;

	// List of lines in .bss section
	std::list<std::string> variables;

	// List of procedures
	std::list<Procedure> procedures;
};

#endif // ASMSTRUCTURE_H_INCLUDED
