// Defines the members of the Interpreter class

#include <iostream>
#include <sstream>
#include <string>

#include "boost/optional.hpp"

#include "Interpreter.h"
#include "exception.h"
#include "mixedModeFilter.h"

// Constructor
Interpreter::Interpreter(std::ostream& os) :
  out(os),
  cur_var(0),
  it(0),
  environments(0)
{
  environments.push_front(std::make_unique<Environment<VarData>>());
}

// Interpreter error definition
void Interpreter::error(const Token& t, const std::string& msg)
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

// Accepts a StmtList reference
void Interpreter::visit(StmtList& node)
{
	// Run the statements
  for (std::shared_ptr<Stmt> s: node.get_stmts())
    s->accept(*this);
}

// Accepts a BasicIf reference
void Interpreter::visit(BasicIf& node)
{
  node.get_if()->accept(*this);

  bool expr = boost::get<bool>(it);

  if (expr)
    node.get_if_stmts()->accept(*this);

  it = expr;
}

// Accepts a IfStmt reference
void Interpreter::visit(IfStmt& node)
{
  node.get_if()->accept(*this); // The if statement always exists

  // Whenever 'it' is true, then we ran statements and need to leave this node
  if (boost::get<bool>(it))
    return;

  if (!node.get_elseifs().empty())
    for (auto ifs: node.get_elseifs())
    {
      ifs->accept(*this);
      if (boost::get<bool>(it))
        return;
    }

  if (node.get_else())
    node.get_else()->accept(*this);
}

// Accepts a WhileStmt reference
void Interpreter::visit(WhileStmt& node)
{
  node.get_while()->accept(*this);

  while (boost::get<bool>(it))
  {
    node.get_stmts()->accept(*this);
    node.get_while()->accept(*this);
  }
}

// Accepts a PrintStmt reference
void Interpreter::visit(PrintStmt& node)
{
  node.get_expr()->accept(*this);

  it = boost::apply_visitor(string_visitor(), it);

  switch (node.get_type())
  {
  case TokenType::PRINT:
    std::cout << it;
    break;
  case TokenType::PRINTLN:
    std::cout << it << std::endl;
    break;

  default: break;
  }
}

// Accepts a VarDecStmt reference
void Interpreter::visit(VarDecStmt& node)
{
  cur_var = new VarData(node.get_type(), node.get_sub_type()); // Create the base VarData object

  if (node.get_assign())
  {
    node.get_assign()->accept(*this); // Fill 'it' with the value of the rhs
    cur_var->set_value(it); // Set the value to VarData
  }

  environments.front()->add_identifier(node.get_id().get_lexeme(), (*cur_var));
}

// Accepts an AssignStmt reference
void Interpreter::visit(AssignStmt& node)
{
  // First, get the VarData for the variable we are assigning to
  std::unique_ptr<VarData>* vardata;

  for (const auto& e: environments)  /// update to std::for_each
  { // Check if this environment has this identifier defined
    std::unique_ptr<VarData>* var_data = e->get_identifier(node.get_id().get_lexeme());
    if (var_data != nullptr)
    {
      vardata = var_data;
      break;
    }
  }

  int index = 0; // The index to write to

  if (node.get_index()) // Then we are writing to a particular element of a list
  {
    node.get_index()->accept(*this); // Determines what index we are writing to
    index = boost::get<int>(it);
  }

  node.get_assign()->accept(*this); // Get the value that should be assigned

  if (!(*vardata)->set_value(it, index)) // Set the new value to the
    error(node.get_id(), "Out of bounds access, ");
}

// A helper to make SimpleExpr easier to navigate
boost::optional<all_type> SimpleExprHelper(TokenType type, all_type data)
{
  all_type ret; // The return object

  switch (type)
  {
  case TokenType::BOOL:
    ret = boost::apply_visitor(bool_visitor(), data);
    break;

  case TokenType::INT:
    ret = boost::apply_visitor(int_visitor(), data);
    break;

  case TokenType::STRING:
    ret = boost::apply_visitor(string_visitor(), data);
    break;

  default:
    return boost::none;
    break;
  }

  return ret;
}

// Accepts a SimpleExpr reference
void Interpreter::visit(SimpleExpr& node)
{
  all_type raw = node.get_term().get_lexeme(); // Conversion of string to all_type

  boost::optional<all_type> var = SimpleExprHelper(node.get_term().get_type(), raw); // Checking

  if (!var)
  { // The expression is for an identifier
    for (const auto& e: environments)
    { // Check if this environment has this identifier defined
      std::unique_ptr<VarData>* var_data = e->get_identifier(node.get_term().get_lexeme());
      if (var_data != nullptr)
      {
        string_visitor sv = string_visitor(); // Needed for apply_visitor
        var = SimpleExprHelper((*var_data)->get_type(), (*var_data)->get_value().apply_visitor(sv)); // Cannot fail
      }
    }
  }

  it = *var; // Remove the value from the optional
}

// Accepts a IndexExpr reference
void Interpreter::visit(IndexExpr& node)
{
  node.get_expr()->accept(*this); // Load 'it' with the index value to access

  for (const auto& e: environments)
  { // Check if this environment has this identifier defined
    std::unique_ptr<VarData>* vardata = e->get_identifier(node.get_id().get_lexeme());
    if (vardata != nullptr)
    {
      int_visitor iv = int_visitor(); // Needed for apply_visitor
      it = (*vardata)->get_value(it.apply_visitor(iv));
    }
  }
}

// Accepts a ListExpr reference
void Interpreter::visit(ListExpr& node)
{
  cur_var->set_length(node.get_exprs().size());

  int i = 0;
  for (auto& e: node.get_exprs())
  {
    e->accept(*this);
    if (!cur_var->set_value(it, i))
      std::cerr << "Failed to set value " << it << " to index " << i << "." << std::endl;
    ++i;
  }

  node.get_exprs().at(0)->accept(*this); // Small hack to avoid issues with VarDecStmt resetting index 0 to 'it'
}

// Accepts a ReadExpr reference
void Interpreter::visit(ReadExpr& node)
{
  switch (node.get_type())
  {
    case TokenType::READINT:
    { // Reading an int
      int in;
      std::cout << node.get_msg().get_lexeme();
      std::cin >> in;
      it = in;
      break;
    }

    case TokenType::READSTR:
    { // Reading a string
      std::string in;
      std::cout << node.get_msg().get_lexeme();
      std::cin >> in;
      it = in;
      break;
    }

    default: break;
  }
}

// Accepts a ComplexExpr reference
void Interpreter::visit(ComplexExpr& node)
{
  node.get_first_op()->accept(*this);

  all_type first = it;

  node.get_rest()->accept(*this);

  it = mixed_mode_math_filter(first, it, node.get_rel().get_type());
}

// Accepts a SimpleBoolExpr reference
void Interpreter::visit(SimpleBoolExpr& node)
{
  node.get_expr_term()->accept(*this);
}

// Accepts a ComplexBoolExpr reference
void Interpreter::visit(ComplexBoolExpr& node)
{
  node.get_first_op()->accept(*this);
  all_type first = it; // Store the result of the first part
  node.get_second_op()->accept(*this);

  if (node.get_rest())
  {
    first = mixed_mode_bool_filter(first, it, node.get_rel()); // store the result of the lhs of the boolean connector
    node.get_rest()->accept(*this); // Get the rest of the boolean expression

    // We know that both first and it are boolean types because they must be so in a boolean expression
    // so it is safe to use get here.
    switch (node.get_con_type())
    {
      case TokenType::OR:
        it = boost::get<bool>(first) || boost::get<bool>(it);
        break;

      case TokenType::AND:
        it = boost::get<bool>(first) && boost::get<bool>(it);
        break;

      default: break;
    }
  }
  else // We are at the end of the boolean expression
    it = mixed_mode_bool_filter(first, it, node.get_rel());
}

// Accepts a NotBoolExpr reference
void Interpreter::visit(NotBoolExpr& node)
{
  node.get_expr()->accept(*this);
  it = !boost::get<bool>(it);
}
