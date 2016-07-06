// Defines the members of the PrintVisitor class

#include <iostream>

#include "PrintVisitor.h"

// Constructor
PrintVisitor::PrintVisitor(std::ostream& os) :
  out(os),
  indents(0),
  indent(std::begin(indents))
{
  // Place the initial indent level in the container
  indents.push_back(std::string(INDENT));
  // Point the iterator to the initial element
  indent = std::begin(indents);
}

// Accepts a StmtList reference
void PrintVisitor::visit(StmtList& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "StmtList: " << (node.get_stmts().size() == 0 ? "empty" : "") << std::endl;

  // Increase the indent
  next_indent();

  // Print out the statements
  for (std::shared_ptr<Stmt> s : node.get_stmts())
    s->accept(*this);

  // Return the previous indent
  prev_indent();
}

// Accepts a BasicIf reference
void PrintVisitor::visit(BasicIf& node)
{
  // Print out the if statement's boolean expression
  node.get_if()->accept(*this);

  // Return the previous indent
  prev_indent();

  // Print out the then statements
  out << *indent + "THEN:" << std::endl;

  // Increase the indent
  next_indent();

  // Print the executable statements
  node.get_if_stmts()->accept(*this);
}

// Accepts a IfStmt reference
void PrintVisitor::visit(IfStmt& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "IfSmt: " << std::endl;
  next_indent();

  // Print out the first if clause
  out << *indent << "IF: " << std::endl;
  // Increase the indent
  next_indent();
  // Print the BasicIf
  node.get_if()->accept(*this);
  // Return to the previous indent
  prev_indent();

  // Print out all of the else if clauses
  for (std::shared_ptr<BasicIf> b : node.get_elseifs())
  {
    out << *indent << "ELSE IF: " << std::endl;
    // Increase the indent
    next_indent();
    // Print the executable statements
    b->accept(*this);
    // Return to the previous indent
    prev_indent();
  }

  // Print out the else clause
  if (node.get_else())
  {
    out << *indent << "ELSE: " << std::endl;
    // Increase the indent
    next_indent();
    // Print the executable statements
    node.get_else()->accept(*this);
    // Return to the previous indent
    prev_indent();
  }

  // Return to the previous indent
  prev_indent();
}

// Accepts a WhileStmt reference
void PrintVisitor::visit(WhileStmt& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "WhileStmt: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out the condition
  out << *indent << "WHILE:" << std::endl;
  // Increase the indent
  next_indent();
  // Print the boolean expression
  node.get_while()->accept(*this);
  // Return to the previous indent
  prev_indent();

  // Print out the statements
  out << *indent << "STMTS:" << std::endl;
  // Increase the indent
  next_indent();
  // Print the executable statements
  node.get_stmts()->accept(*this);
  // Return to the previous indent
  prev_indent();

  // Return to the previous indent
  prev_indent();
}

// Accepts a PrintStmt reference
void PrintVisitor::visit(PrintStmt& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "PrintStmt: " << node.get_type() << std::endl;

  // Increase the indent
  next_indent();

  // Print out the expression to print
  node.get_expr()->accept(*this);

  // Return to the previous indent
  prev_indent();
}

// Accepts a VarDecStmt reference
void PrintVisitor::visit(VarDecStmt& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "VarDecStmt: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out what to assign to
  out << *indent << "ID: " << node.get_id().get_lexeme() << std::endl
      << *indent << "TYPE: " << node.get_type();
  if (node.get_sub_type() != TokenType::UNKNOWN)
    out << node.get_sub_type();
  out << std::endl;

  if (node.get_assign())
  {
    // Print out the value to be assigned
    out << *indent << "VALUE:" << std::endl;
    // Increase the indent
    next_indent();
    // Print the expression to be assigned
    node.get_assign()->accept(*this);
    // Increase the indent
    prev_indent();
  }

  // Return to the previous indent
  prev_indent();
}

// Accepts a AssignStmt reference
void PrintVisitor::visit(AssignStmt& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "AssignStmt: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out what to assign to
  out << *indent << (node.get_index() ? "INDEXED ID: " : "ID: ") << node.get_id().get_lexeme() << std::endl;
  if (node.get_index())
  {
    // Increase the indent
    next_indent();
    // Print out the index expression
    node.get_index()->accept(*this);
    // Increase the indent
    prev_indent();
  }

  // Print out the value to be assigned
  out << *indent << "VALUE:" << std::endl;
  // Increase the indent
  next_indent();
  // Print the expression to be assigned
  node.get_assign()->accept(*this);
  // Increase the indent
  prev_indent();

  // Return to the previous indent
  prev_indent();
}

// Accepts a SimpleExpr reference
void PrintVisitor::visit(SimpleExpr& node)
{
  // Print out what type of ASTNode this is...
  out << *indent << "SimpleExpr: " << node.get_term().get_type()
  // ...and the value of the expression
      << " (" << node.get_term().get_lexeme() << ")" << std::endl;
}

// Accepts a IndexExpr reference
void PrintVisitor::visit(IndexExpr& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "IndexExpr: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out the id
  out << *indent << "INDEXED ID (" << node.get_id().get_lexeme() << ")" << std::endl;

  // Increase the indent
  next_indent();
  // Print out the expression that determines what element to access
  node.get_expr()->accept(*this);
  // Return to the previous indent
  prev_indent();

  // Return to the previous indent
  prev_indent();
}

// Accepts a ListExpr reference
void PrintVisitor::visit(ListExpr& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "ListExpr: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out the list of expressions
  for (unsigned i = 0; i < node.get_exprs().size(); i++)
  {
    out << *indent << "EXPR" << i << ":" << std::endl;
    // Increase the indent
    next_indent();
    // Print out the list expressions
    node.get_exprs()[i]->accept(*this);
    // Return to the previous indent
    prev_indent();
  }

  // Return to the previous indent
  prev_indent();
}

// Accepts a ReadExpr reference
void PrintVisitor::visit(ReadExpr& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "ReadExpr: " << node.get_type()
  // Print out the prompt to display
      << " ('" << node.get_msg().get_lexeme() << "')" << std::endl;
}

// Accepts a ComplexExpr reference
void PrintVisitor::visit(ComplexExpr& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "ComplexExpr: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out the first operand...
  node.get_first_op()->accept(*this);
  // ...the mathematical relation...
  out << *indent << node.get_rel() << std::endl;
  // ...and the rest
  node.get_rest()->accept(*this);

  // Return to the previous indent
  prev_indent();
}

// Accepts a SimpleBoolExpr reference
void PrintVisitor::visit(SimpleBoolExpr& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "SimpleBoolExpr:" << std::endl;

  // Increase the indent
  next_indent();

  // Print out the expression
  node.get_expr_term()->accept(*this);

  // Return to the previous indent
  prev_indent();
}

// Accepts a ComplexBoolExpr reference
void PrintVisitor::visit(ComplexBoolExpr& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "ComplexBoolExpr: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out the first operand...
  node.get_first_op()->accept(*this);
  // ...the boolean relation...
  out << *indent << node.get_rel() << std::endl;
  // ...the second operand..
  node.get_second_op()->accept(*this);

  // ...the boolean connector, and the rest if they are set
  if (node.get_rest() && node.get_con_type() != TokenType::UNKNOWN)
  {
    out << *indent << node.get_con_type() << std::endl;
    node.get_rest()->accept(*this);
  }

  // Return to the previous indent
  prev_indent();
}

// Accepts a NotBoolExpr reference
void PrintVisitor::visit(NotBoolExpr& node)
{
  // Print out what type of ASTNode this is
  out << *indent << "NotBoolExpr: " << std::endl;

  // Increase the indent
  next_indent();

  // Print out the BoolExpr being negated
  node.get_expr()->accept(*this);

  // Return to the previous indent
  prev_indent();
}
