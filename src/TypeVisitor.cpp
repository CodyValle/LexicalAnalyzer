// Defines everything that is declared in TypeVisitor.h

#include <iostream>
#include <sstream>

#include "TypeVisitor.h"
#include "exception.h"

// TypeVisitor constructor
TypeVisitor::TypeVisitor(std::ostream& os) :
  out(os),
  expr_type(TokenType::UNKNOWN),
  expr_sub_type(TokenType::UNKNOWN),
  list_len(0),
  environments(0)
{
  environments.push_front(std::make_unique<Environment<IDData>>());
}

// TypeVisitor print_knowledge definition
void TypeVisitor::print_knowledge()
{
  // Iterate and print environments
  for (const auto& e: environments)
    std::cout << *e.get();
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
std::unique_ptr<IDData>* TypeVisitor::found_identifier(Token t, bool reading)
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
      return data;
    }
  }

  // The identifier wasn't found
  error(t, "Use of undeclared identifier, ");
  return nullptr;
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
  environments.push_front(std::make_unique<Environment<IDData>>());

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
    environments.push_front(std::make_unique<Environment<IDData>>());

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
  environments.push_front(std::make_unique<Environment<IDData>>());

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
  // Pointer to the IDData
  IDData* data = nullptr;

  // Check the assignment
  if (node.get_assign())
  {
    expr_type = node.get_type(); // Sets the current type to what was declared

    // Get the type of the assign statement
    node.get_assign()->accept(*this);
    if (node.get_type() != TokenType::VAR)
    { // The type was explicitly declared
      // Check that the right hand side matches
      if (expr_type != node.get_type() || expr_sub_type != node.get_sub_type())
        error(node.get_id(), "mismatched explicit type and implicit type");
    }

    // Make the IDData
    data = new IDData(true, expr_type, expr_sub_type, list_len);

    list_len = 0;
    expr_sub_type = TokenType::UNKNOWN;
  }
  else
  { // This is an explicit type declaration with no assignment
    data = new IDData(false, node.get_type(),node.get_sub_type());
  }

  // Add this variable to the local environment
  if (!environments.front()->add_identifier(node.get_id().get_lexeme(), (*data)))
    // There is already a variable declared in this scope with the same lexeme
    error(node.get_id(), "redefinition of variable");
}

// TypeVisitor AssignStmt visit definition
void TypeVisitor::visit(AssignStmt& node)
{
  // Report that this variable is being assigned to
  found_identifier(node.get_id(), false);

  // Check the index expression
  if (node.get_index())
  {
    node.get_index()->accept(*this);

    // Make sure the index expression returned an integer
    if (expr_type != TokenType::INT)
      error(node.get_id(), "expected integer value as index ");
  }

  // Check the expression to be assigned
  node.get_assign()->accept(*this);

  // Check that the expression has the same type as what it is being assigned to
  for (const auto& e: environments)
  {
    // Check if this environment has this identifier defined
    std::unique_ptr<IDData>* data = e->get_identifier(node.get_id().get_lexeme());
    if (data != nullptr)
    {
      if (expr_type != (*data)->get_type())
        error(node.get_id(), "cannot assign rhs type to lhs identifier ");
    }
  }
  // The identifier will be found since the earlier call to found_identifier succeeded
}

// TypeVisitor SimpleExpr visit definition
void TypeVisitor::visit(SimpleExpr& node)
{
  switch (node.get_term().get_type())
  {
  case TokenType::BOOL:
  case TokenType::INT:
  case TokenType::STRING:
    expr_type = node.get_term().get_type();
    break;

  case TokenType::ID:
  {
    std::unique_ptr<IDData>* data = found_identifier(node.get_term());
    expr_type = (*data)->get_type();
    expr_sub_type = (*data)->get_sub_type();
    break;
  }

  default: break;
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
  // Set the sub type
  expr_sub_type = TokenType::ARRAY;

  // Get the expressions
  std::deque<std::shared_ptr<Expr>> exprs = node.get_exprs();

  // exprs could be empty
  if (exprs.empty())
  {
    // If it is, then the type should be explicit
    if (expr_type == TokenType::VAR)
      error(node.get_lbracket(), "undeterminable type. Cannot infer the type of this list ");
    else return;
  }

  // Set the length of the list
  list_len = exprs.size();

  // Start the iterator
  std::deque<std::shared_ptr<Expr>>::iterator it = exprs.begin();

  // Get the type of the first expression
  (*it)->accept(*this);
  ++it;

  // Save for later
  TokenType type = expr_type;

  // Iterate through and make sure they are all the same type
  for (; it != exprs.end(); ++it)
  {
    (*it)->accept(*this);
    if (type != expr_type)
      error(node.get_lbracket(), "mismatched types in list initializer ");
  }
}

// TypeVisitor ReadExpr visit definition
void TypeVisitor::visit(ReadExpr& node)
{
  // What type of read operation is this?
  switch (node.get_type())
  {
    // Gets an integer
  case TokenType::READINT:
    expr_type = TokenType::INT;
    break;

    // Gets a string
  case TokenType::READSTR:
    expr_type = TokenType::STRING;
    break;

  default: break;
  }
}

// TypeVisitor ComplexExpr visit definition
void TypeVisitor::visit(ComplexExpr& node)
{
  // Get the type of the first operand
  node.get_first_op()->accept(*this);
  TokenType type = expr_type;

  // Check that the rest of the expression is the same type
  node.get_rest()->accept(*this);

  // Check for type consistency
  if (expr_type != type)
  {
    // Check for mixed-mode expressions
    switch (type)
    {
    case TokenType::STRING:
      if (expr_type == TokenType::INT)
      { // String and int can be mixed
        // But only by addition and multiplication
        if (node.get_rel().get_type() == TokenType::PLUS
            || node.get_rel().get_type() == TokenType::MULTIPLY)
        {
          // These are of type string
          expr_type = TokenType::STRING;
          return;
        }
      }
      break;

    default: break;
    }

    // Cannot mix these types
    error(node.get_rel(), "mismatched types in complex expression ");
  }

  // Check that proper expressions types are being operated on
  switch (expr_type)
  {
    // Strings can only be added
  case TokenType::STRING:
    if (node.get_rel().get_type() != TokenType::PLUS)
      error(node.get_rel(), "illegal operation. Strings can only be added ");
    break;

    // Booleans can only be added or subtracted
  case TokenType::BOOL:
    if (node.get_rel().get_type() != TokenType::PLUS && node.get_rel().get_type() != TokenType::MINUS)
      error(node.get_rel(), "illegal operation. Booleans can only be added or subtracted ");
    break;

  default: break;
  }
}

// TypeVisitor SimpleBoolExpr visit definition
void TypeVisitor::visit(SimpleBoolExpr& node)
{
  // Get the type of this expression
  node.get_expr_term()->accept(*this);

  // It ought to be a boolean
  if (expr_type != TokenType::BOOL)
    error(node.get_token(), "non-boolean expression in simple boolean expression");
}

// TypeVisitor ComplexBoolExpr visit definition
void TypeVisitor::visit(ComplexBoolExpr& node)
{
  // Get the type of the first operand
  node.get_first_op()->accept(*this);
  TokenType type = expr_type;

  // Check that the second operand is the same type
  node.get_second_op()->accept(*this);
  if (expr_type != type)
    error(node.get_token(), "mismatched types in complex boolean expression");

  // Check that the rest of the expression is the same type
  if (node.get_rest())
    node.get_rest()->accept(*this);
}

// TypeVisitor NotBoolExpr visit definition
void TypeVisitor::visit(NotBoolExpr& node)
{
  // Check the expression being negated
  node.get_expr()->accept(*this);

  // Make sure it'a boolean expression
  if (expr_type != TokenType::BOOL)
    error(node.get_token(), "non-boolean expression in negation");
}
