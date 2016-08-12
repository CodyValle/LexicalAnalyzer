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

	// Print procedures/subprograms
	std::for_each(procedures.begin(), procedures.end(),
			[&](Procedure* p) { out << *p << std::endl; });
}

// Add a string constant to the program
void AsmStructure::add_constant(std::string name, std::string data)
{
	constants.push_back(name +": db '" + data + "'");
	constants.push_back(name +"len: equ $-" + name); // Length of the string
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

// Adds a buffer variable to the program
void AsmStructure::add_buffer_variable()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add the buffer
		add_variable("buffer", 255);
	}
}

// Add a procedure to the program
void AsmStructure::add_procedure(Procedure* p)
{
	procedures.push_back(p);
}

// Adds the print procedure to the program
void AsmStructure::add_print_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add some constants
		add_constant("LF", 10, true);

		// Add the print buffer
		add_buffer_variable();

		// Add the print procedure
		Procedure* proc = new Procedure("print");
		proc->add_instruction("mov rax,4");
		proc->add_instruction("mov rbx,1");
		proc->add_instruction("mov rcx,buffer");
		proc->add_instruction("int 80h");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the sprint procedure to the program
void AsmStructure::add_sprint_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add dependencies
		add_strlen_proc();

		// Add the procedure
		Procedure* proc = new Procedure("sprint");
		proc->add_instruction("push edx");
		proc->add_instruction("push ecx");
		proc->add_instruction("push ebx");
		proc->add_instruction("push eax");
		proc->add_instruction("call strlen");
		proc->add_instruction("mov edx,eax");
		proc->add_instruction("pop eax");
		proc->add_instruction("mov ecx,eax");
		proc->add_instruction("mov ebx,1");
		proc->add_instruction("mov eax,4");
		proc->add_instruction("int 80h");
		proc->add_instruction("pop ebx");
		proc->add_instruction("pop ecx");
		proc->add_instruction("pop edx");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the iprint procedure to the program
void AsmStructure::add_iprint_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add dependencies
		add_sprint_proc();

		// Add the procedure
		Procedure* proc = new Procedure("iprint");
		proc->add_instruction("push eax");
		proc->add_instruction("push ecx");
		proc->add_instruction("push edx");
		proc->add_instruction("push esi");
		proc->add_instruction("mov ecx,0");
		proc->add_instruction(".divideLoop:");
		proc->add_instruction("inc ecx");
		proc->add_instruction("mov edx,0");
		proc->add_instruction("mov esi,10");
		proc->add_instruction("idiv esi");
		proc->add_instruction("add edx,48");
		proc->add_instruction("push edx");
		proc->add_instruction("cmp eax,0");
		proc->add_instruction("jnz .divideLoop");
		proc->add_instruction(".printLoop:");
		proc->add_instruction("dec ecx");
		proc->add_instruction("mov eax,esp");
		proc->add_instruction("call sprint");
		proc->add_instruction("pop eax");
		proc->add_instruction("cmp ecx,0");
		proc->add_instruction("jnz .printLoop");
		proc->add_instruction("pop esi");
		proc->add_instruction("pop edx");
		proc->add_instruction("pop ecx");
		proc->add_instruction("pop eax");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the bprint procedure to the program
void AsmStructure::add_bprint_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add dependencies
		add_constant("boolt", "true");
		add_constant("boolf", "false");
		add_sprint_proc();

		// Add the procedure
		Procedure* proc = new Procedure("bprint");
		proc->add_instruction("cmp eax,0");
		proc->add_instruction("je .false");
		proc->add_instruction("mov eax,boolt");
		proc->add_instruction("call sprint");
		proc->add_instruction("ret");
		proc->add_instruction(".false");
		proc->add_instruction("mov eax,boolf");
		proc->add_instruction("call sprint");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the printLF procedure to the program
void AsmStructure::add_printLF_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add dependencies
		add_sprint_proc();

		// Add the procedure
		Procedure* proc = new Procedure("printLF");
		proc->add_instruction("push eax");
		proc->add_instruction("mov eax, 0Ah");
		proc->add_instruction("push eax");
		proc->add_instruction("mov eax,esp");
		proc->add_instruction("call sprint");
		proc->add_instruction("pop eax");
		proc->add_instruction("pop eax");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the strlen procedure to the program
void AsmStructure::add_strlen_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add the procedure
		Procedure* proc = new Procedure("strlen");
		proc->add_instruction("push ebx");
		proc->add_instruction("mov ebx,eax");
		proc->add_instruction(".next:");
		proc->add_instruction("cmp byte[eax],0");
		proc->add_instruction("jz .finished");
		proc->add_instruction("inc eax");
		proc->add_instruction("jmp .next");
		proc->add_instruction(".finished:");
		proc->add_instruction("sub eax,ebx");
		proc->add_instruction("pop ebx");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the readstr procedure to the program
void AsmStructure::add_readstr_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add the procedure
		Procedure* proc = new Procedure("readstr");
		proc->add_instruction("mov edx, 255");
		proc->add_instruction("mov ecx,eax");
		proc->add_instruction("mov ebx,0");
		proc->add_instruction("mov eax,3");
		proc->add_instruction("int 80h");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the readint procedure to the program
void AsmStructure::add_readint_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add dependencies
		add_buffer_variable();

		// Add the procedure
		Procedure* proc = new Procedure("readint");
		proc->add_instruction("mov edx, 255");
		proc->add_instruction("mov ecx,buffer");
		proc->add_instruction("mov ebx,0");
		proc->add_instruction("mov eax,3");
		proc->add_instruction("int 80h");
		proc->add_instruction("mov eax,buffer");
		proc->add_instruction("call atoi");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the atoi procedure to the program
void AsmStructure::add_atoi_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add the procedure
		Procedure* proc = new Procedure("atoi");
		proc->add_instruction("push ebx");
		proc->add_instruction("push ecx");
		proc->add_instruction("push edx");
		proc->add_instruction("push esi");
		proc->add_instruction("mov esi,eax");
		proc->add_instruction("mov eax,0");
		proc->add_instruction("mov ecx,0");
		proc->add_instruction(".multiplyLoop:");
		proc->add_instruction("xor ebx,ebx");
		proc->add_instruction("mov bl,[esi+ecx]");
		proc->add_instruction("cmp bl,48");
		proc->add_instruction("jl  .finished");
		proc->add_instruction("cmp bl,57");
		proc->add_instruction("jg  .finished");
		proc->add_instruction("cmp bl,10");
		proc->add_instruction("je .finished");
		proc->add_instruction("cmp bl,0");
		proc->add_instruction("jz .finished");
		proc->add_instruction("sub bl,48");
		proc->add_instruction("add eax,ebx");
		proc->add_instruction("mov ebx,10");
		proc->add_instruction("mul ebx");
		proc->add_instruction("inc ecx");
		proc->add_instruction("jmp .multiplyLoop");
		proc->add_instruction(".finished:");
		proc->add_instruction("mov ebx,10");
		proc->add_instruction("div ebx");
		proc->add_instruction("pop esi");
		proc->add_instruction("pop edx");
		proc->add_instruction("pop ecx");
		proc->add_instruction("pop ebx");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}

// Adds the strcpy procedure to the program
void AsmStructure::add_strcpy_proc()
{
  // Run this code only once
	static bool added = false;
	if (!added)
	{
		added = true; // Prevent this from running again

		// Add the procedure
		Procedure* proc = new Procedure("strcpy");
		proc->add_instruction(";TODO");
		proc->add_instruction("ret");
		add_procedure(proc);
	}
}
