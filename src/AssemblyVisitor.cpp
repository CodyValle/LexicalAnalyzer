// Defines the members of the AssemblyVisitor class

#include <iostream>
#include <sstream>
#include <string>

#include "boost/optional.hpp"

#include "AssemblyVisitor.h"
#include "token.h"
#include "ast.h"

// Constructor
AssemblyVisitor::AssemblyVisitor() :
	asms(0),
	proc(0),
	mode(Mode::PRINT),
	id_map()
{
	asms = new AsmStructure();
}

// Outputs the assembly
void AssemblyVisitor::output(std::ostream& os)
{
	asms->convert(os);
}

// Accepts a StmtList reference
void AssemblyVisitor::visit(StmtList& node)
{
	// Start a new procedure
	static bool initialized = false; // local variables
	static unsigned count = 1;

	if (!initialized)
	{ // First time through
		initialized = true;
		proc = new Procedure("_start");
	}
	else
	{ // All other times
		proc = new Procedure("proc" + std::to_string(count));
	}

	Procedure* local_proc = proc;

	// Run the statements
	for (std::shared_ptr<Stmt> s: node.get_stmts())
		s->accept(*this);

	// Add the procedure to the AsmStructure
	asms->add_procedure(*local_proc);
}

// Accepts a BasicIf reference
void AssemblyVisitor::visit(BasicIf& node)
{
	node.get_if()->accept(*this);
	node.get_if_stmts()->accept(*this);
}

// Accepts a IfStmt reference
void AssemblyVisitor::visit(IfStmt& node)
{
	node.get_if()->accept(*this);
	for (auto& expr: node.get_elseifs())
		expr->accept(*this);
	if (node.get_else())
		node.get_else()->accept(*this);
}

// Accepts a WhileStmt reference
void AssemblyVisitor::visit(WhileStmt& node)
{
	node.get_while()->accept(*this);
	node.get_stmts()->accept(*this);
}

// Accepts a PrintStmt reference
void AssemblyVisitor::visit(PrintStmt& node)
{
	// Inform what is going on
	mode = Mode::PRINT;

	// Run some code only once
	static bool print_added = false;
	if (!print_added)
	{
		print_added = true; // Prevent this from running again

		// Add some constants
		asms->add_constant("LF", 10, true);

		// Add the print buffer
		asms->add_variable("printbuf", 255);

		// Add the print procedure
		Procedure* print_proc = new Procedure("print");
		print_proc->add_instruction("mov rax,4");
		print_proc->add_instruction("mov rbx,1");
		print_proc->add_instruction("mov rcx,printbuf");
		print_proc->add_instruction("int 80h");
		print_proc->add_instruction("ret");
		asms->add_procedure(*print_proc);
	}

	// Initial stuff to prepare for printing
	proc->add_instruction("mov rcx,printbuf");
	proc->add_instruction("mov rdx,0");

	// Set up printing the argument
	node.get_expr()->accept(*this); // Loads the value into rax

	// Check if there should be a newline afterward
	if (node.get_type() == TokenType::PRINTLN)
	{
		proc->add_instruction("mov byte [rcx],LF");
		proc->add_instruction("add rcx,1");
		proc->add_instruction("add rdx,1");
	}

	// Do the print
	proc->add_instruction("call print");
}

// Accepts a VarDecStmt reference
void AssemblyVisitor::visit(VarDecStmt& node)
{
	/// Used for adding global constants
	// Get the type
	//...
	//asms->add_constant(node.get_id().get_lexeme(), (std::string)"Hello, world!");

	// Keep track of type information
	Type type = Type::INT;

	// Adds a global variable
	/// Needs to check whether local or global
	switch (node.get_type())
	{
	case TokenType::INT:
		asms->add_variable(node.get_id().get_lexeme(), 1);
		break;

	case TokenType::BOOL:
		type = Type::BOOL;
		asms->add_variable(node.get_id().get_lexeme(), 1);
		break;

	case TokenType::STRING:
		type = Type::STRING;
		asms->add_variable(node.get_id().get_lexeme(), 255);
		break;

	default: break;
	}

	// Store type information
	id_map[node.get_id().get_lexeme()] = type;

	if (node.get_assign())
	{
		mode = Mode::ASSIGN;
		node.get_assign()->accept(*this); // Loads the value into rax

		proc->add_instruction("mov [" + node.get_id().get_lexeme() + "],rax");
	}
}

// Accepts an AssignStmt reference
void AssemblyVisitor::visit(AssignStmt& node)
{
	if (node.get_index())
		node.get_index()->accept(*this);
	node.get_assign()->accept(*this);
}

// Accepts a SimpleExpr reference
void AssemblyVisitor::visit(SimpleExpr& node)
{
	//std::cerr << "CurMode: " << (mode == Mode::PRINT ? "PRINT" : "ASSIGN") << std::endl;
	//std::cerr << node.get_term() << " " << node.get_term().get_type() << std::endl;

	switch (mode)
	{
	case Mode::PRINT:
		switch (node.get_term().get_type())
		{
		case TokenType::ID: // For printing an identifier
		{
			// We have to determine if this is an int, bool, or string identifier
			switch (id_map[node.get_term().get_lexeme()])
			{
			case INT:
				proc->add_instruction("mov rax,[" + node.get_term().get_lexeme() + "]");
				proc->add_instruction("add rax,0x30");
				proc->add_instruction("mov [rcx],rax");
				proc->add_instruction("add rcx,1");
				proc->add_instruction("add rdx,1");
				break;

			case BOOL:
				std::cerr << "BOOL" << std::endl;
				break;

			case STRING:
				std::cerr << "STRING" << std::endl;
				break;
			}
			break;
		}

		case TokenType::INT:
			proc->add_instruction("mov rax," + node.get_term().get_lexeme());
			proc->add_instruction("add rax,0x30");
			proc->add_instruction("mov [rcx],rax");
			proc->add_instruction("add rcx,1");
			proc->add_instruction("add rdx,1");
			break;


		case TokenType::BOOL:
			break;


		case TokenType::STRING:
			break;

		default: break;
		}
		break;


	case Mode::ASSIGN:
		switch (node.get_term().get_type())
		{
		case TokenType::ID: // For assigning an identifier
			switch (node.get_term().get_type())
			{
			case TokenType::INT:
				break;

			case TokenType::BOOL:
				break;

			case TokenType::STRING:
				break;

			default: break;
			}
			break;


		case TokenType::INT:
			proc->add_instruction("mov rax," + node.get_term().get_lexeme());
			break;


		case TokenType::BOOL:
			break;


		case TokenType::STRING:
			break;

		default: break;
		}
		break;


	default: break;
	}
}

// Accepts a IndexExpr reference
void AssemblyVisitor::visit(IndexExpr& node)
{
	node.get_expr()->accept(*this);
}

// Accepts a ListExpr reference
void AssemblyVisitor::visit(ListExpr& node)
{
	for (auto& expr: node.get_exprs())
		expr->accept(*this);
}

// Accepts a ReadExpr reference
void AssemblyVisitor::visit(ReadExpr& node)
{
}

// Accepts a ComplexExpr reference
void AssemblyVisitor::visit(ComplexExpr& node)
{
	node.get_first_op()->accept(*this); // Loads the first expression into rax
	proc->add_instruction("mov r10,rax"); // Saves the value from the first op
	node.get_rest()->accept(*this); // Loads the rest into rax

	switch (node.get_rel().get_type())
	{
	case TokenType::PLUS:
		proc->add_instruction("add rax,r10");
		break;

	case TokenType::MINUS:
		proc->add_instruction("sub rax,r10");
		break;

	case TokenType::DIVIDE:
		proc->add_instruction("div rax,r10");
		break;

	case TokenType::MULTIPLY:
		proc->add_instruction("mul rax,r10");
		break;

	default: break;
	}
}

// Accepts a SimpleBoolExpr reference
void AssemblyVisitor::visit(SimpleBoolExpr& node)
{
	node.get_expr_term()->accept(*this);
}

// Accepts a ComplexBoolExpr reference
void AssemblyVisitor::visit(ComplexBoolExpr& node)
{
	node.get_first_op()->accept(*this);
	node.get_second_op()->accept(*this);
	if (node.get_rest())
		node.get_rest()->accept(*this);
}

// Accepts a NotBoolExpr reference
void AssemblyVisitor::visit(NotBoolExpr& node)
{
	node.get_expr()->accept(*this);
}
