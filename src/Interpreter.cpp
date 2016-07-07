// Defines the members of the Interpreter class

#include <iostream>

#include "Interpreter.h"

// Constructor
Interpreter::Interpreter(std::ostream& os) :
  out(os),
  environments(0)
{
	//std::unique_ptr<Environment<IDData>> pt = std::make_unique<Environment<IDData>>();
  environments.push_front(std::make_unique<Environment<IDData>>());
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
}

// Accepts a IfStmt reference
void Interpreter::visit(IfStmt& node)
{
}

// Accepts a WhileStmt reference
void Interpreter::visit(WhileStmt& node)
{
}

// Accepts a PrintStmt reference
void Interpreter::visit(PrintStmt& node)
{
}

// Accepts a VarDecStmt reference
void Interpreter::visit(VarDecStmt& node)
{

}

// Accepts a AssignStmt reference
void Interpreter::visit(AssignStmt& node)
{
}

// Accepts a SimpleExpr reference
void Interpreter::visit(SimpleExpr& node)
{
}

// Accepts a IndexExpr reference
void Interpreter::visit(IndexExpr& node)
{
}

// Accepts a ListExpr reference
void Interpreter::visit(ListExpr& node)
{
}

// Accepts a ReadExpr reference
void Interpreter::visit(ReadExpr& node)
{
}

// Accepts a ComplexExpr reference
void Interpreter::visit(ComplexExpr& node)
{
}

// Accepts a SimpleBoolExpr reference
void Interpreter::visit(SimpleBoolExpr& node)
{
}

// Accepts a ComplexBoolExpr reference
void Interpreter::visit(ComplexBoolExpr& node)
{
}

// Accepts a NotBoolExpr reference
void Interpreter::visit(NotBoolExpr& node)
{
}
