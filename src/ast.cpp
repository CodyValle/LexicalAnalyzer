// Defines all necessary functions for all the nodes of the AST

#include <iostream>
#include "ast.h"

// The default INDENT level
const std::string INDENT = " ";


//----------------------------------------------------------------------
// StmtList
//----------------------------------------------------------------------

// StmtList print definition
void StmtList::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "StmtList: " << (stmts.size() == 0 ? "empty" : "") << std::endl;

  // Print out the statements
  for (std::shared_ptr<Stmt> s : stmts)
    s->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// BasicIf
//----------------------------------------------------------------------

// BasicIf print definition
void BasicIf::print(std::ostream& out, std::string indent)
{
  // Print out the if statement's boolean expression
  if_expr->print(out, indent + INDENT);

  // Print out the then statements
  out << indent + "THEN:" << std::endl;
  if_stmts->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// IfStmt
//----------------------------------------------------------------------

// IfStmt print definition
void IfStmt::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "IfSmt: " << std::endl;

  // Print out the first if clause
  out << indent + INDENT << "IF: " << std::endl;
  if_part->print(out, indent + INDENT);

  // Print out all of the else if clauses
  for (std::shared_ptr<BasicIf> b : elseifs)
  {
    out << indent + INDENT << "ELSE IF: " << std::endl;
    b->print(out, indent + INDENT);
  }

  // Print out the else clause
  if (else_stmts)
  {
    out << indent + INDENT << "ELSE: " << std::endl;
    else_stmts->print(out, indent + INDENT);
  }
}

//----------------------------------------------------------------------
// WhileStmt
//----------------------------------------------------------------------

// WhileStmt print definition
void WhileStmt::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "WhileStmt: " << std::endl;

  // Print out the condition
  while_expr->print(out, indent + INDENT);
  // Print out the statements
  while_stmts->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// PrintStmt
//----------------------------------------------------------------------

// PrintStmt constructor
PrintStmt::PrintStmt() :
  print_type(TokenType::PRINTLN)  // Default to PRINTLN
{
}

// PrintStmt print definition
void PrintStmt::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "PrintStmt: " << print_type << std::endl;

  // Print out the expression to print
  print_expr->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// AssignStmt
//----------------------------------------------------------------------

// AssignStmt print definition
void AssignStmt::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "AssignStmt: " << std::endl;

  // Print out what to assign to
  out << indent + INDENT << (index_expr ? "INDEXED ID: " : "ID: ") << id.get_lexeme() << std::endl;
  if (index_expr) index_expr->print(out, indent + INDENT);

  // Print out the value to be assigned
  out << indent + INDENT << "VALUE:" << std::endl;
  assign_expr->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// SimpleExpr
//----------------------------------------------------------------------

// SimpleExpr print definition
void SimpleExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is...
  out << indent << "SimpleExpr: " << term.get_type()
  // ...and the value of the expression
      << " (" << term.get_lexeme() << ")" << std::endl;
}

//----------------------------------------------------------------------
// IndexExpr
//----------------------------------------------------------------------

// IndexExpr print definition
void IndexExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "IndexExpr: " << std::endl;

  // Print out the id
  out << indent + INDENT << "INDEXED ID (" << id.get_lexeme() << ")" << std::endl;
  // Print out the expression that determines what element to access
  index_expr->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// ListExpr
//----------------------------------------------------------------------

// ListExpr print definition
void ListExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "ListExpr: " << std::endl;

  // Print out the list of expressions
  for (unsigned i = 0; i < expr_list.size(); i++)
  {
    out << indent + INDENT << "EXPR" << i << ": ";
    expr_list[i]->print(out, "");
  }
}

//----------------------------------------------------------------------
// ReadExpr
//----------------------------------------------------------------------

// ReadExpr constructor
ReadExpr::ReadExpr() :
  read_type(TokenType::READSTR) // Default to READSTR
{
}

// ReadExpr print definition
void ReadExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "ReadExpr: " << read_type
  // Print out the prompt to display
      << " ('" << msg.get_lexeme() << "')" << std::endl;
}

//----------------------------------------------------------------------
// ComplexExpr
//----------------------------------------------------------------------

// ComplexExpr print definition
void ComplexExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "ComplexExpr: " << std::endl;

  // Print out the first operand...
  first_op->print(out, indent + INDENT);
  // ...the mathematical relation...
  out << indent + INDENT << math_rel << std::endl;
  // ...and the rest
  rest->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// SimpleBoolExpr
//----------------------------------------------------------------------

// SimpleBoolExpr print definition
void SimpleBoolExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "SimpleBoolExpr:" << std::endl;

  // Print out he expression
  expr_term->print(out, indent + INDENT);
}

//----------------------------------------------------------------------
// ComplexBoolExpr
//----------------------------------------------------------------------

// ComplexBoolExpr constructor
ComplexBoolExpr::ComplexBoolExpr() :
  bool_con_type(TokenType::UNKNOWN)
{
}

// ComplexBoolExpr print definition
void ComplexBoolExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "ComplexBoolExpr: " << std::endl;

  // Print out eh first operand...
  first_op->print(out, indent + INDENT);
  // ...the boolean relation...
  out << indent + INDENT << bool_rel << std::endl;
  // ...the second operand..
  second_op->print(out, indent + INDENT);

  // ...the boolean connector and the rest, if they are set
  if (rest && bool_con_type != TokenType::UNKNOWN)
  {
    out << indent + INDENT << bool_con_type << std::endl;
    rest->print(out, indent + INDENT);
  }
}

//----------------------------------------------------------------------
// NotBoolExpr
//----------------------------------------------------------------------

// NotBoolExpr print definition
void NotBoolExpr::print(std::ostream& out, std::string indent)
{
  // Print out what type of ASTNode this is
  out << indent << "NotBoolExpr: " << std::endl;
  // Print out the BoolExpr being negated
  bool_expr->print(out, indent + INDENT);
}
