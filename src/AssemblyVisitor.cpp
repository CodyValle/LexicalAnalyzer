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
	type(Type::INT),
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
	static unsigned count = 0; // Number of procedures created
	Procedure* local_proc; // Keep track of which procedure this function needs to close

	if (!initialized)
	{ // First time through
		initialized = true;
		local_proc = proc = new Procedure("_start");
	}
	else
	{ // All other times
	  proc->add_instruction("call proc" + std::to_string(count)); // Call the procedure about to be created
		local_proc = proc = new Procedure("proc" + std::to_string(count++));
	}

	// Add the procedure to the AsmStructure
	asms->add_procedure(local_proc);

	// Run the statements
	for (std::shared_ptr<Stmt> s: node.get_stmts())
		s->accept(*this); // May modify proc

  // Close the procedure, as long as it isn't _start
  if (local_proc->get_name().compare("_start") != 0)
    local_proc->add_instruction("ret");
  else
  { // CLose _start
    local_proc->add_instruction("xor ebx,ebx");
    local_proc->add_instruction("quit: mov eax,1");
    local_proc->add_instruction("int 80h");
  }
}

// Accepts a BasicIf reference
void AssemblyVisitor::visit(BasicIf& node)
{
  static unsigned count = 0;
  std::string label = "iflbl" + std::to_string(count++); // Label for this if statement
  Procedure* local_proc = proc; // Keep track of the local procedure

	node.get_if()->accept(*this); // Loads eax with a boolean value
	proc->add_instruction("push eax"); // Save the bool for later
	proc->add_instruction("cmp eax,0"); // Performs comparison
	proc->add_instruction("je " + label); // Jumps if 0 (false)
	node.get_if_stmts()->accept(*this); // Add statements
	proc = local_proc; // Restore the original state
	proc->add_instruction(label + ":"); // The label to jump to if false
	proc->add_instruction("pop eax"); // Get the bool back
}

// Accepts a IfStmt reference
void AssemblyVisitor::visit(IfStmt& node)
{

  static unsigned count = 0;
  std::string label = "ifblock" + std::to_string(count++); // Label for this if statement
  Procedure* local_proc = proc; // Keep track of the local procedure

	node.get_if()->accept(*this); // Run the BasicIf

	if (node.get_elseifs().size() > 0) // Then we add them with some other code
  {
    proc->add_instruction("cmp eax,1"); // Performs comparison
    proc->add_instruction("je " + label); // Jumps if 1 (true) because the if statement was ran

    for (auto& expr: node.get_elseifs())
    {
      expr->accept(*this); // Run the elseif (BasicIf)
      proc->add_instruction("cmp eax,1"); // Performs comparison
      proc->add_instruction("je " + label); // Jumps if 1 (true) because an elseif statement was ran
    }
  }

	if (node.get_else())
  {
    proc->add_instruction("cmp eax,1"); // Performs comparison
      proc->add_instruction("je " + label); // Jumps if 1 (true) because an if block ran
    node.get_else()->accept(*this);
    proc = local_proc; // Restore the original state
  }

	proc->add_instruction(label + ":"); // The label to jump to when an if is true
}

// Accepts a WhileStmt reference
void AssemblyVisitor::visit(WhileStmt& node)
{
  static unsigned count = 0; // Number of while loops
  std::string label = "whileloop" + std::to_string(count++);
  Procedure* local_proc = proc; // Keep track of the local procedure

  proc->add_instruction(label + ":"); // Remain local

	node.get_while()->accept(*this); // Loads eax with a bool
	proc->add_instruction("cmp eax,0"); // Compare with false
	proc->add_instruction("je done" + label); // Escape if false

	node.get_stmts()->accept(*this); // Create a procedure for these statement
	proc = local_proc; // Restore the original state
	proc->add_instruction("jmp " + label); // Run again
	proc->add_instruction("done" + label + ":"); // The end of this while loop
}

// Accepts a PrintStmt reference
void AssemblyVisitor::visit(PrintStmt& node)
{
	// Set up printing the argument
	node.get_expr()->accept(*this); // Loads the value into eax, and determines type

	// What are we printing?
	switch (type)
	{
  case INT:
    asms->add_uiprint_proc();
    proc->add_instruction("call uiprint");
    break;

  case BOOL:
    asms->add_bprint_proc();
    proc->add_instruction("call bprint");
    break;

  case STRING:
    asms->add_sprint_proc();
    proc->add_instruction("call sprint"); // The address is already loaded into eax
    break;
	}

	// Print line feed if necessary
	if (node.get_type() == TokenType::PRINTLN)
  {
    asms->add_printLF_proc();
    proc->add_instruction("call printLF");
  }
}

// Accepts a VarDecStmt reference
void AssemblyVisitor::visit(VarDecStmt& node)
{
	/// Used for adding global constants
	// Get the type
	//...
	//asms->add_constant(node.get_id().get_lexeme(), (std::string)"Hello, world!");

	// Keep track of type information
	Type var_type = Type::INT;

	// Adds a global variable
	/// Needs to check whether local or global
	switch (node.get_type())
	{
	case TokenType::BOOL:
		var_type = Type::BOOL;
		asms->add_variable(node.get_id().get_lexeme(), 1);
		break;

	case TokenType::INT:
		asms->add_variable(node.get_id().get_lexeme(), 4); // 32-bit numbers
		break;

	case TokenType::STRING:
		var_type = Type::STRING;
		asms->add_variable(node.get_id().get_lexeme(), 255);
		break;

	default: break;
	}

	// Store type information
	id_map[node.get_id().get_lexeme()] = var_type;

	if (node.get_assign())
	{
    node.get_assign()->accept(*this); // Loads the value into the eax
    switch (type)
    {
    case INT:
    case BOOL:
      proc->add_instruction("mov [" + node.get_id().get_lexeme() + "],eax"); // Move the value into the variable
      break;

    case STRING:
      proc->add_instruction("push eax"); // Store the address
      proc->add_instruction("mov eax," + node.get_id().get_lexeme()); // Write to the variable address
      proc->add_instruction("pop ebx"); // Load where to read from
      asms->add_strcpy_proc();
      proc->add_instruction("call strcpy"); // Copy the string over
      break;
    }
	}
}

// Accepts an AssignStmt reference
void AssemblyVisitor::visit(AssignStmt& node)
{
  /// Does not handle lists yet
	if (node.get_index())
		node.get_index()->accept(*this);

  node.get_assign()->accept(*this); // Loads eax with the value to store
  switch (type)
  {
  case INT:
  case BOOL: /// bools are only one byte; this won't work
    proc->add_instruction("mov [" + node.get_id().get_lexeme() + "],eax"); // Save it
    break;

  case STRING:
    proc->add_instruction("mov ebx,eax"); // Where to read from
    proc->add_instruction("mov eax," + node.get_id().get_lexeme()); // Write to the variable address
    asms->add_strcpy_proc();
    proc->add_instruction("call strcpy"); // Copy the string over
    break;

  default: break;
  }
}

// Accepts a SimpleExpr reference
void AssemblyVisitor::visit(SimpleExpr& node)
{
  switch (node.get_term().get_type())
  {
  case TokenType::ID:
    type = id_map[node.get_term().get_lexeme()]; // Assign type
    switch (type)
    {
    case INT:
    case BOOL:
      proc->add_instruction("mov eax,[" + node.get_term().get_lexeme() + "]"); // Move known data to eax
      break;

    case STRING:
      proc->add_instruction("mov eax," + node.get_term().get_lexeme() + ""); // Move address of data to eax
      break;
    }
    break;

  case TokenType::INT:
    type = Type::INT;
    proc->add_instruction("mov eax," + node.get_term().get_lexeme());
    break;


  case TokenType::BOOL:
    type = Type::BOOL;
    if (node.get_term().get_lexeme().compare("true") == 0)
      proc->add_instruction("mov eax,1"); // 1 is true
    else
      proc->add_instruction("mov eax,0"); // 0 is false
    break;


  case TokenType::STRING:
  {
    static unsigned count = 0; // Keep track of the number of constant strings
    type = Type::STRING;

    // Load the string address into eax
    asms->add_constant("strconst" + std::to_string(count), node.get_term().get_lexeme());
    proc->add_instruction("mov eax,strconst" + std::to_string(count++));
    break;
  }

  default: break;
  }
}

// Accepts a IndexExpr reference
void AssemblyVisitor::visit(IndexExpr& node)
{
  /// Does not handle lists yet
	node.get_expr()->accept(*this);
}

// Accepts a ListExpr reference
void AssemblyVisitor::visit(ListExpr& node)
{
  /// Does not handle lists yet
	for (auto& expr: node.get_exprs())
		expr->accept(*this);
}

// Accepts a ReadExpr reference
void AssemblyVisitor::visit(ReadExpr& node)
{
  // Number of read expressions
  static unsigned count = 0;

  // Add needed procedures
  asms->add_sprint_proc();

  // Print the message
  std::string name = "read" + std::to_string(count++);
  asms->add_constant(name, node.get_msg().get_lexeme());
  proc->add_instruction("mov eax," + name); // Load the address of the print message
  proc->add_instruction("call sprint"); // Print message

  // Get the user's input and
  // convert to the proper type, if necessary
  switch (type)
  {
  case INT:
    asms->add_readint_proc();
    proc->add_instruction("call readint"); // Reads the input and puts the value in eax
    break;

  case BOOL:
    // There is no readbool function.
    break;

  case STRING:
    asms->add_readstr_proc();
    proc->add_instruction("call readstr"); // Loads eax with the address of a filled buffer
    break;
  }
}

// Accepts a ComplexExpr reference
void AssemblyVisitor::visit(ComplexExpr& node)
{
	node.get_first_op()->accept(*this); // Loads the first expression into eax
	proc->add_instruction("push eax"); // Saves the value from the first op
	Type first_type = type;
	node.get_rest()->accept(*this); // Loads the rest into eax

	// Perform the arithmetical operation
	switch (first_type)
	{ /// All unsupported cases can be removed. They will never be called due to previous type checks.
  case INT: // int is the first argument
  case BOOL: // bool is the first argument
    switch (type)
    {
    case INT: // (int | bool) REL int
    case BOOL: // (int | bool) REL bool
      switch (node.get_rel().get_type())
      {
      case TokenType::PLUS: // (int | bool) + (int | bool)
        proc->add_instruction("pop ebx");
        proc->add_instruction("add eax,ebx");
        break;

      case TokenType::MINUS: // (int | bool) - (int | bool)
        proc->add_instruction("mov ebx,eax");
        proc->add_instruction("pop eax");
        proc->add_instruction("sub eax,ebx");
        break;

      case TokenType::DIVIDE: // (int | bool) / (int | bool)
        proc->add_instruction("mov ebx,eax");
        proc->add_instruction("pop eax");
        proc->add_instruction("div ebx");
        break;

      case TokenType::MULTIPLY:  // (int | bool) / (int | bool)
        proc->add_instruction("pop ebx");
        proc->add_instruction("mul ebx");
        break;

      default: break;
      }
      break;

    case STRING: // (int | bool) REL string
      // Unsupported
      break;
    }
    break;

  case STRING: // string is the first argument
    switch (type)
    {
    case INT: // string REL int
      switch (node.get_rel().get_type())
      {
      case TokenType::PLUS: // string + int
        type = STRING;
        asms->add_itoa_proc();
        proc->add_instruction("call itoa");
        proc->add_instruction("mov ebx,eax"); // Address of the string of integers into ebx
        proc->add_instruction("pop eax"); // Get the first operand back
        asms->add_append_proc();
        proc->add_instruction("call append");
        break;

      case TokenType::MULTIPLY: // string * int
        proc->add_instruction("mov ebx,eax");
        proc->add_instruction("pop eax");
        asms->add_strmulint_proc();
        proc->add_instruction("call strmulint");
        break;

      case TokenType::MINUS: // string - int
      case TokenType::DIVIDE: // string / int
        // Unsupported
        break;

      default: break;
      }
      break;

    case BOOL: // string REL bool
      switch (node.get_rel().get_type())
      {
      case TokenType::PLUS: // string + bool
        proc->add_instruction("mov ebx,eax");
        proc->add_instruction("pop eax");
        asms->add_straddbool_proc();
        proc->add_instruction("call straddbool");
        break;

      case TokenType::MULTIPLY: // string * bool
        proc->add_instruction("mov ebx,eax");
        proc->add_instruction("pop eax");
        proc->add_instruction("strmulbool:");
        proc->add_instruction("cmp ebx,1"); // is the bool true?
        proc->add_instruction("jne .done");
        proc->add_instruction("mov [eax],0");
        proc->add_instruction(".done");
        break;

      case TokenType::MINUS: // string - bool
      case TokenType::DIVIDE: // string / bool
        // Unsupported
        break;

      default: break;
      }
      break;

    case STRING: // string REL string
      switch (node.get_rel().get_type())
      {
      case TokenType::PLUS: // string + string
        proc->add_instruction("mov ebx,eax");
        proc->add_instruction("pop eax"); // Pull the first argument off the stack
        asms->add_append_proc();
        proc->add_instruction("call append"); // Appends ebx to eax
        break;

      case TokenType::MINUS: // string - string
      case TokenType::DIVIDE: // string * string
      case TokenType::MULTIPLY: // string / string
        // Unsupported
        break;

      default: break;
      }
      break;
    }
    break;
	}
}

// Accepts a SimpleBoolExpr reference
void AssemblyVisitor::visit(SimpleBoolExpr& node)
{
	node.get_expr_term()->accept(*this); // Pretty simple
}

// Accepts a ComplexBoolExpr reference
void AssemblyVisitor::visit(ComplexBoolExpr& node)
{
  static unsigned count = 0; // Count the comparisons
	node.get_first_op()->accept(*this); // Loads eax with the first operand

	proc->add_instruction("push eax"); // Save for later
	Type first_type = type; // Get the type of the first operand

	node.get_second_op()->accept(*this); // Load eax with the second operand

  proc->add_instruction("comparison" + std::to_string(count++) + ":"); // Remain local
	// Load eax with the result
	switch (first_type)
	{ /// All unsupported cases can be removed. They will never be called due to previous type checks.
  case INT: // integer is the first operand
    proc->add_instruction("pop ebx"); // Get the first operand
    switch (type)
    {
    case INT: // integer REL integer
      proc->add_instruction("cmp ebx,eax"); // Compare the two
      switch (node.get_rel())
      {
      case TokenType::EQUAL: // int == int
        proc->add_instruction("je .comptrue");
        break;

      case TokenType::NOT_EQUAL: // int != int
        proc->add_instruction("jne .comptrue");
        break;

      case TokenType::GREATER_THAN: // int < int
        proc->add_instruction("jg .comptrue");
        break;

      case TokenType::GREATER_THAN_EQUAL: // int <= int
        proc->add_instruction("jge .comptrue");
        break;

      case TokenType::LESS_THAN: // int > int
        proc->add_instruction("jl .comptrue");
        break;

      case TokenType::LESS_THAN_EQUAL: // int >= int
        proc->add_instruction("jle .comptrue");
        break;

      default: break;
      }
      break;

    case BOOL: // integer REL boolean
    case STRING: // integer REL string
      // Unsupported
      break;
    }
    break;

  case BOOL: // bool is the first operand
    // Unsupported
    break;

  case STRING: // string is the first operand
    switch (type)
    {
    case INT: // string REL integer
    case BOOL: // string REL boolean
      // Unsupported
      break;

    case STRING: // string REL string
      proc->add_instruction("mov ebx, eax"); // Second operand in ebx
      proc->add_instruction("pop eax"); // Get the first operand
      asms->add_strcmp_proc();
      proc->add_instruction("call strcmp"); // Loads eax with -1 (lt), 0 (eq), 1(gt)
      proc->add_instruction("cmp eax,0");
      switch (node.get_rel())
      {
      case TokenType::EQUAL: // string == string
        proc->add_instruction("je .comptrue");
        break;

      case TokenType::NOT_EQUAL: // string != string
        proc->add_instruction("jne .comptrue");
        break;

      case TokenType::GREATER_THAN: // string < string
        proc->add_instruction("jl .comptrue");
        break;

      case TokenType::GREATER_THAN_EQUAL: // string <= string
        proc->add_instruction("jle .comptrue");
        break;

      case TokenType::LESS_THAN: // string > string
        proc->add_instruction("jg .comptrue");
        break;

      case TokenType::LESS_THAN_EQUAL: // string >= string
        proc->add_instruction("jge .comptrue");
        break;

      default: break;
      }
      break;
    }
    break;
	}
  // Same for all cases
  proc->add_instruction("mov eax,0");
  proc->add_instruction("jmp .done");
  proc->add_instruction(".comptrue:");
  proc->add_instruction("mov eax,1");
  proc->add_instruction(".done:");

	// Do boolean connector
	if (node.get_rest())
  {
    proc->add_instruction("push eax"); // Save for later
    node.get_rest()->accept(*this);
    proc->add_instruction("pop ebx"); // Get the first operand
    proc->add_instruction("connector:"); // Remain local

    proc->add_instruction("add eax,ebx"); // Same for all cases
    switch (node.get_con_type())
    {
    case TokenType::AND:
      proc->add_instruction("cmp eax,2");
      proc->add_instruction("jne .connfalse");
      break;

    case TokenType::OR:
      proc->add_instruction("cmp eax,0");
      proc->add_instruction("je .connfalse");
      break;

    default: break;
    }
    // Same for all cases
    proc->add_instruction("mov eax,1");
    proc->add_instruction("jmp .done");
    proc->add_instruction(".connfalse:");
    proc->add_instruction("mov eax,0");
    proc->add_instruction(".done");
  }
}

// Accepts a NotBoolExpr reference
void AssemblyVisitor::visit(NotBoolExpr& node)
{
	node.get_expr()->accept(*this); // Load eax with bool
  proc->add_instruction("not:"); // Remain local
  proc->add_instruction("cmp eax,0");
  proc->add_instruction("jne .false"); // Invert value
  proc->add_instruction("mov eax,1");
  proc->add_instruction("jmp .done");
  proc->add_instruction(".false:");
  proc->add_instruction("mov eax,0");
  proc->add_instruction(".done");
}
