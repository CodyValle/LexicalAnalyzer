// Defines everything in AsmStructure.h

#include <algorithm>
#include <sstream>

#include "AsmStructure.h"

// Constructor
Procedure::Procedure(std::string n) :
	name(n)
{
}

// Print operator for a Procedure object
std::ostream& operator<<(std::ostream& out, const Procedure& proc)
{
	out << proc.name << ":" << std::endl;
	for (std::string s: proc.instructions)
		out << s << std::endl;
	return out;
}

// Constructor
AsmStructure::AsmStructure() :
	constants(),
	variables(),
	procedures()
{
}

// Converts the structure into assembly and puts it in the passed in stream
void AsmStructure::convert(std::ostream& out)
{
	// .data section first
	if (constants.size() > 0)
	{
		out << "section .data" << std::endl;
		std::for_each(constants.begin(), constants.end(),
			[&](std::string& s) { out << s << std::endl; }); // Print each line to the stream

		out << std::endl; // End of section
	}

	// .bss section second
	if (variables.size() > 0)
	{
		out << "section .bss" << std::endl;
		std::for_each(variables.begin(), variables.end(),
				[&](std::string& s) { out << s << std::endl; }); // Print each line to the stream

		out << std::endl; // End of section
	}

	// .text section last
	out << "section .text" << std::endl
		<< "global _start" << std:: endl << std::endl;

	// Add closing statements to first procedure
	for (auto& p: procedures)
		if (p.get_name().compare("_start") == 0)
		{
			p.add_instruction("mov rax,1");
			p.add_instruction("end: mov rbx,0");
			p.add_instruction("int 80h");
		}

	// Print procedures/subprograms
	std::for_each(procedures.begin(), procedures.end(),
			[&](Procedure& p) { out << p << std::endl; });
}

// Add a string constant to the program
void AsmStructure::add_constant(std::string name, std::string data)
{
	constants.push_back(name +": db '" + data + "'");
}

// Add an integer constant to the program
void AsmStructure::add_constant(std::string name, int data, bool eq)
{
	constants.push_back(name +": " + (eq ? "equ" : "dw") + " " + std::to_string(data));
}

// Add a boolean constant to the program
void AsmStructure::add_constant(std::string name, bool data)
{
	constants.push_back(name +": db " + (data ? "1" : "0"));
}

// Add a variable to the program
void AsmStructure::add_variable(std::string name, int size)
{
	variables.push_back(name +": resb " + std::to_string(size));
}

// Add a procedure to the program
void AsmStructure::add_procedure(Procedure& p)
{
	procedures.push_back(p);
}
