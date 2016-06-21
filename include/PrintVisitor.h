#ifndef PRINTVISITOR_H_INCLUDED
#define PRINTVISITOR_H_INCLUDED

// Declares the PrintVisitor class

#include <deque>

#include "ast.h"

// The PrintVisitor class prints out the AST in an understandable form
class PrintVisitor : public AbstractVisitor
{
public:
  // Constructor
  // Takes an output stream to print to
  PrintVisitor(std::ostream&);

  // The overridden functions from AbstractVisitor
  void visit(StmtList&) override;
  void visit(BasicIf&) override;
  void visit(IfStmt&) override;
  void visit(WhileStmt&) override;
  void visit(PrintStmt&) override;
  void visit(VarDecStmt&) override;
  void visit(AssignStmt&) override;
  void visit(SimpleExpr&) override;
  void visit(IndexExpr&) override;
  void visit(ListExpr&) override;
  void visit(ReadExpr&) override;
  void visit(ComplexExpr&) override;
  void visit(SimpleBoolExpr&) override;
  void visit(ComplexBoolExpr&) override;
  void visit(NotBoolExpr&) override;

private:
  // Reference to the output stream
  std::ostream& out;

  /// These members act as an on-demand indent length pool

  // THe length of every indent
  const std::string INDENT = " ";

  // Stores strings of various lengths of INDENT
  std::deque<std::string> indents;
  // Points at the current indent in the container
  std::deque<std::string>::iterator indent;

  // Moves the pointer to the next largest indent
  // Dynamically increases the indent container appropriately
  void next_indent()
    {
      // Check if a new one is needed
      if (std::next(indent) == indents.end())
        indents.push_back(std::string(*indent + INDENT));

      ++indent;
    }

  // Moves to a previous indent
  void prev_indent()
    { --indent; }
};

#endif // PRINTVISITOR_H_INCLUDED
