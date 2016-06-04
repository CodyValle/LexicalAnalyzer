// Defines everything that is declared in VariableVisitor.h

#include <iostream>
#include <sstream>

#include "VariableVisitor.h"
#include "exception.h"

// IDData constructor
IDData::IDData(bool init, TokenType type) :
  initialized(init),
  data_type(type)
{
}

// VariableVisitor constructor
VariableVisitor::VariableVisitor(std::ostream& os) :
  out(os),
  identifiers(0)
{
}

// VariableVisitor print_knowledge definition
void VariableVisitor::print_knowledge()
{
  // Iterate and print keys and IDData stored in identifiers
  for (const auto& i : identifiers)
  {
    out << "Identifier:[" << i.first << "]" << std::endl
        << "  Initialized:[" << (i.second->is_initialized() == 0 ? "false" : "true") << "]" << std::endl
        << "  Data Type:  [" << i.second->get_type() << "]"
        << std::endl;
  }
}

// VariableVisitor error definition
void VariableVisitor::error(const Token& t, const std::string& msg)
{
	// Create the error message
	std::ostringstream error_str;
	error_str << msg << t.get_lexeme() << ".";

	// Throw the error
	throw Exception(error_str.str(),
									t.get_line(),
									t.get_column(),
									ExceptionType::VARVISIT);
}

void VariableVisitor::found_identifier(Token token, TokenType type)
{
  // We need to check that it exists in the table
  if (identifiers.count(token.get_lexeme()))
  { // The identifier is in the table
    // Check if it used uninitialized
    if (!identifiers[token.get_lexeme()]->is_initialized())
      error(token, "Use of uninitialized identifier, ");
  }
  else
  { // The identifier is not in the table
    // Check that it has a type declaration
    switch (type)
    {
    case TokenType::UNKNOWN:
      // We have an error
      error(token, "No type declaration for ");

    case TokenType::DEFAULT:
      // Using a token that is uninitialized
      error(token, "Use of uninitialized identifier, ");

    default: break;
    }

    // Safe to assume the type declaration is proper
    // Add it to the table
    identifiers[token.get_lexeme()] = new IDData(true, type);
  }
}

// VariableVisitor StmtList visit definition
void VariableVisitor::visit(StmtList& node)
{
  // Check all the statements
  for (auto& s: node.get_stmts())
    s->accept(*this);
}

// VariableVisitor BasicIf visit definition
void VariableVisitor::visit(BasicIf& node)
{
  // Check the if statement's boolean expression
  node.get_if()->accept(*this);

  // Check the executable statements
  node.get_if_stmts()->accept(*this);
}

// VariableVisitor IfStmt visit definition
void VariableVisitor::visit(IfStmt& node)
{
  // Check the BasicIf
  node.get_if()->accept(*this);

  // Check all of the else if clauses
  for (auto& b : node.get_elseifs())
    b->accept(*this);

  // Check the else statements
  if (node.get_else())
    node.get_else()->accept(*this);
}

// VariableVisitor WhileStmt visit definition
void VariableVisitor::visit(WhileStmt& node)
{
  // Check the boolean expression
  node.get_while()->accept(*this);

  // Check the executable statements
  node.get_stmts()->accept(*this);
}

// VariableVisitor PrintStmt visit definition
void VariableVisitor::visit(PrintStmt& node)
{
  // Check the expression to print
  node.get_expr()->accept(*this);
}

// VariableVisitor AssignStmt visit definition
void VariableVisitor::visit(AssignStmt& node)
{
  // Add the identifier to the table
  found_identifier(node.get_id(), node.get_type());

  // Check the index expression
  if (node.get_index())
    node.get_index()->accept(*this);

  // Check the expression to be assigned
  node.get_assign()->accept(*this);
}

// VariableVisitor SimpleExpr visit definition
void VariableVisitor::visit(SimpleExpr& node)
{
  // Only if this is a Token for an identifier
  if (node.get_term().get_type() == TokenType::ID)
  { // This identifier is being used
    found_identifier(node.get_term(), TokenType::DEFAULT);
  }
}

// VariableVisitor IndexExpr visit definition
void VariableVisitor::visit(IndexExpr& node)
{
  // Add the identifier to the table
  found_identifier(node.get_id(), TokenType::DEFAULT);

  // Check the expression that determines what element to access
  node.get_expr()->accept(*this);
}

// VariableVisitor ListExpr visit definition
void VariableVisitor::visit(ListExpr& node)
{
  // Check the list of expressions
  for (unsigned i = 0; i < node.get_exprs().size(); i++)
    node.get_exprs()[i]->accept(*this);
}

// VariableVisitor ComplexExpr visit definition
void VariableVisitor::visit(ComplexExpr& node)
{
  // Check the first operand...
  node.get_first_op()->accept(*this);
  // ...and the rest
  node.get_rest()->accept(*this);
}

// VariableVisitor SimpleBoolExpr visit definition
void VariableVisitor::visit(SimpleBoolExpr& node)
{
  // Print out the expression
  node.get_expr_term()->accept(*this);
}

// VariableVisitor ComplexBoolExpr visit definition
void VariableVisitor::visit(ComplexBoolExpr& node)
{
  // Check the first operand...
  node.get_first_op()->accept(*this);
  // ...the second operand..
  node.get_second_op()->accept(*this);
  // ...the boolean connector, and the rest if they are set
  if (node.get_rest() && node.get_con_type() != TokenType::UNKNOWN)
    node.get_rest()->accept(*this);
}

// VariableVisitor NotBoolExpr visit definition
void VariableVisitor::visit(NotBoolExpr& node)
{
  // Check the BoolExpr being negated
  node.get_expr()->accept(*this);
}
