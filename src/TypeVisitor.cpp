// Defines everything that is declared in TypeVisitor.h

#include <iostream>
#include <sstream>

#include "TypeVisitor.h"
#include "exception.h"

// TypeVisitor constructor
TypeVisitor::TypeVisitor(std::ostream& os) :
  out(os),
  environments(0)
{
  environments.push_front(std::make_unique<Environment>());
}

// TypeVisitor print_knowledge definition
void TypeVisitor::print_knowledge()
{
  // Iterate and print environments
  for (const auto& e: environments)
    std::cout << (*e.get());
}

// TypeVisitor error definition
void TypeVisitor::error(const Token& t, const std::string& msg)
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

// TypeVisitor found_identifier definition
void TypeVisitor::found_identifier(Token t, bool reading)
{
  // We need to loop through from the beginning to the end (local to global)
  for (const auto& e: environments)
  {
    // Check if this environment has this identifier defined
    std::unique_ptr<IDData>* data = e->get_identifier(t.get_lexeme());
    if (data != nullptr)
    { // The identifier was found
      // Check if we are reading its value uninitialized
      if (reading && !(*data)->is_initialized())
          error(t, "Use of uninitialized identifier, ");
      else // It's being assigned to, so set it to initialized
        (*data)->initialize();

      // No need to search anymore
      return;
    }
  }

  // The identifier wasn't found
  error(t, "Use of undeclared identifier, ");
}

// TypeVisitor StmtList visit definition
void TypeVisitor::visit(StmtList& node)
{
  // Check all the statements
  for (auto& s: node.get_stmts())
    s->accept(*this);
}

// TypeVisitor BasicIf visit definition
void TypeVisitor::visit(BasicIf& node)
{
  // Check the if statement's boolean expression
  node.get_if()->accept(*this);

  // The statements in the if clause are part of a new environment
  environments.push_front(std::make_unique<Environment>());

  // Check the executable statements
  node.get_if_stmts()->accept(*this);

  // We are leaving the if clause, remove the environment
  environments.pop_front();
}

// TypeVisitor IfStmt visit definition
void TypeVisitor::visit(IfStmt& node)
{
  // Check the BasicIf
  node.get_if()->accept(*this);

  // Check all of the else if clauses
  for (auto& b : node.get_elseifs())
    b->accept(*this);

  // Check the else statements
  if (node.get_else())
  {
    // The statements in the else clause are part of a new environment
    environments.push_front(std::make_unique<Environment>());

    node.get_else()->accept(*this);

    // We are leaving the else clause, remove the environment
    environments.pop_front();
  }
}

// TypeVisitor WhileStmt visit definition
void TypeVisitor::visit(WhileStmt& node)
{
  // Check the boolean expression
  node.get_while()->accept(*this);

  // The statements in the while loop are part of a new environment
  environments.push_front(std::make_unique<Environment>());

  // Check the executable statements
  node.get_stmts()->accept(*this);

  // We are leaving the statements in the while loop, remove the environment
  environments.pop_front();
}

// TypeVisitor PrintStmt visit definition
void TypeVisitor::visit(PrintStmt& node)
{
  // Check the expression to print
  node.get_expr()->accept(*this);
}

// TypeVisitor VarDecStmt visit definition
void TypeVisitor::visit(VarDecStmt& node)
{
  // init will be true if the variable is initialized
  bool init = false;

  // Check the index expression
  if (node.get_assign())
  {
    node.get_assign()->accept(*this);
    init = true;
  }

  // Add this variable to the local environment
  IDData data(init, TokenType::UNKNOWN, TokenType::UNKNOWN);
  environments.front()->add_identifier(node.get_id().get_lexeme(), data);
}

// TypeVisitor AssignStmt visit definition
void TypeVisitor::visit(AssignStmt& node)
{
  // Report that this variable is being assigned to
  found_identifier(node.get_id(), false);

  // Check the index expression
  if (node.get_index())
    node.get_index()->accept(*this);

  // Check the expression to be assigned
  node.get_assign()->accept(*this);
}

// TypeVisitor SimpleExpr visit definition
void TypeVisitor::visit(SimpleExpr& node)
{
  // Only if this is a Token for an identifier
  if (node.get_term().get_type() == TokenType::ID)
  { // This identifier is being used
    found_identifier(node.get_term());
  }
}

// TypeVisitor IndexExpr visit definition
void TypeVisitor::visit(IndexExpr& node)
{
  // Report that this variable is being assigned to
  found_identifier(node.get_id());

  // Check the expression that determines what element to access
  node.get_expr()->accept(*this);
}

// TypeVisitor ListExpr visit definition
void TypeVisitor::visit(ListExpr& node)
{
  // Check the list of expressions
  for (unsigned i = 0; i < node.get_exprs().size(); i++)
    node.get_exprs()[i]->accept(*this);
}

// TypeVisitor ComplexExpr visit definition
void TypeVisitor::visit(ComplexExpr& node)
{
  // Check the first operand...
  node.get_first_op()->accept(*this);
  // ...and the rest
  node.get_rest()->accept(*this);
}

// TypeVisitor SimpleBoolExpr visit definition
void TypeVisitor::visit(SimpleBoolExpr& node)
{
  // Print out the expression
  node.get_expr_term()->accept(*this);
}

// TypeVisitor ComplexBoolExpr visit definition
void TypeVisitor::visit(ComplexBoolExpr& node)
{
  // Check the first operand...
  node.get_first_op()->accept(*this);
  // ...the second operand..
  node.get_second_op()->accept(*this);
  // ...the boolean connector, and the rest if they are set
  if (node.get_rest() && node.get_con_type() != TokenType::UNKNOWN)
    node.get_rest()->accept(*this);
}

// TypeVisitor NotBoolExpr visit definition
void TypeVisitor::visit(NotBoolExpr& node)
{
  // Check the BoolExpr being negated
  node.get_expr()->accept(*this);
}
