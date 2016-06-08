#ifndef TYPEVISITOR_H_INCLUDED
#define TYPEVISITOR_H_INCLUDED

// Declares the TypeVisitor class

#include <forward_list>

#include "ast.h"
#include "environment.h"

// The PrintVisitor class prints out the AST in an understandable form
class TypeVisitor : public AbstractVisitor
{
public:
  // Constructor
  TypeVisitor(std::ostream&);

  // Prints out everything that this class knows about identifiers
  void print_knowledge();

  // Reports the error encountered
  void error(const Token&, const std::string&);

  // Checks for proper usage of an identifier when encountered
  void found_identifier(Token, bool = true);

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
  void visit(ReadExpr&) override {}; // TypeVisitor does not care about ReadExpr
  void visit(ComplexExpr&) override;
  void visit(SimpleBoolExpr&) override;
  void visit(ComplexBoolExpr&) override;
  void visit(NotBoolExpr&) override;

private:
  // Reference to the output stream
  std::ostream& out;

  // Stores references to the environments in a LIFO order
  std::forward_list<std::unique_ptr<Environment>> environments;
};

#endif // TYPEVISITOR_H_INCLUDED
