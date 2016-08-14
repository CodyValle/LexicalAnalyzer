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
		{ instructions.push_back(in); };

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

	// Destructor
	~AsmStructure();

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
	void add_procedure(Procedure*);

	// Adds a buffer variable to the program
	void add_buffer_variable();

	// Adds bool string constants to the program
	void add_bool_constants();

	// Adds the print procedure to the program
	void add_print_proc();

	// Adds the sprint procedure to the program
	void add_sprint_proc();

	// Adds the bprint procedure to the program
	void add_bprint_proc();

	// Adds the uiprint procedure to the program
	void add_uiprint_proc();

	// Adds the sprintLF procedure to the program
	void add_printLF_proc();

	// Adds the strlen procedure to the program
	void add_strlen_proc();

	// Adds the readstr procedure to the program
	void add_readstr_proc();

	// Adds the readint procedure to the program
	void add_readint_proc();

	// Adds the atoi procedure to the program
	void add_atoi_proc();

	// Adds the itoa procedure to the program
	void add_itoa_proc();

	// Adds the strcpy procedure to the program
	void add_strcpy_proc();

	// Adds the strcmp procedure to the program
	void add_strcmp_proc();

	// Adds the append procedure to the program
	void add_append_proc();

	// Adds the strmulint procedure to the program
	void add_strmulint_proc();

	// Adds the straddbool procedure to the program
	void add_straddbool_proc();

private:
	// List of lines in .data section
	std::list<std::string> constants;

	// List of lines in .bss section
	std::list<std::string> variables;

	// List of procedures
	std::list<Procedure*> procedures;
};

#endif // ASMSTRUCTURE_H_INCLUDED
