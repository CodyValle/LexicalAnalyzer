#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

// Declares the Interpreter class

#include <forward_list>

#include "ast.h"
#include "environment.h"
#include "iddata.h"

// The Interpreter class interprets and executes the AST.
class Interpreter : public AbstractVisitor
{
public:
  // Constructor
  // Takes an output stream to print to
  Interpreter(std::ostream&);

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

  // Stores references to the environments in a LIFO order
  std::forward_list<std::unique_ptr<Environment<IDData>>> environments;
};

#endif // INTERPRETER_H_INCLUDED
