#ifndef VARIABLEVISITOR_H_INCLUDED
#define VARIABLEVISITOR_H_INCLUDED

// Declares the VariableVisitor class

#include <unordered_map>

#include "ast.h"

// Stores all known data on the identifier it is linked to
class IDData
{
public:
  // Constructor
  IDData(bool, TokenType);

  // Returns the value of initialized
  bool is_initialized()
    { return initialized; }

  // Get the type of this identifier
  TokenType get_type()
    { return data_type; }

private:
  // Tells whether this variable has been initialized
  bool initialized;

  // The type of this identifier
  TokenType data_type;
};

// The PrintVisitor class prints out the AST in an understandable form
class VariableVisitor : public AbstractVisitor
{
public:
  // Constructor
  VariableVisitor(std::ostream&);

  // Prints out everything that this class knows about identifiers
  void print_knowledge();

  // Reports the error encountered
	void error(const Token&, const std::string&);

	// To check conditions whenever an identifier is found
	void found_identifier(Token, TokenType);

  // The overridden functions from AbstractVisitor
  void visit(StmtList&) override;
  void visit(BasicIf&) override;
  void visit(IfStmt&) override;
  void visit(WhileStmt&) override;
  void visit(PrintStmt&) override;
  void visit(AssignStmt&) override;
  void visit(SimpleExpr&) override;
  void visit(IndexExpr&) override;
  void visit(ListExpr&) override;
  void visit(ReadExpr&) override {}; // VariableVisitor does not care about ReadExpr
  void visit(ComplexExpr&) override;
  void visit(SimpleBoolExpr&) override;
  void visit(ComplexBoolExpr&) override;
  void visit(NotBoolExpr&) override;

private:
  // Reference to the output stream
  std::ostream& out;

  // Stores the identifiers found in the file
  std::unordered_map<std::string, IDData*> identifiers;
};

#endif // VARIABLEVISITOR_H_INCLUDED
