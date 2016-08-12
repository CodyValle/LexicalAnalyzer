#ifndef ASSEMBLYVISITOR_H_INCLUDED
#define ASSEMBLYVISITOR_H_INCLUDED

// Declares the AssemblyVisitor class

#include <unordered_map>

#include "ast.h"
#include "AsmStructure.h"
#include "all_type.h"

enum class Mode
{
	PRINT, ASSIGN
};

class AssemblyVisitor : public AbstractVisitor
{
public:
  // Constructor
  // Takes an output stream to print to
  AssemblyVisitor();

	// Outputs the created structure
	void output(std::ostream&);

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
  // The assembly structure
  AsmStructure* asms;

  // The current procedure being created
  Procedure* proc;

  // The type of the current expression
  Type type;

  // Tracks identifiers and types
  std::unordered_map<std::string,Type> id_map;
};

#endif // ASSEMBLYVISITOR_H_INCLUDED
