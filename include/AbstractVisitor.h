#ifndef ABSTRACTVISITOR_H_INCLUDED
#define ABSTRACTVISITOR_H_INCLUDED

// Forward declarations of AST classes
class StmtList;
class BasicIf;
class IfStmt;
class WhileStmt;
class PrintStmt;
class AssignStmt;
class SimpleExpr;
class IndexExpr;
class ListExpr;
class ReadExpr;
class ComplexExpr;
class SimpleBoolExpr;
class ComplexBoolExpr;
class NotBoolExpr;

// Base class for all Visitor classes
class AbstractVisitor
{
public:
  // Virtual destructor
  virtual ~AbstractVisitor() {};

  // A visit function for every class in an AST
  virtual void visit(StmtList&) = 0;
  virtual void visit(BasicIf&) = 0;
  virtual void visit(IfStmt&) = 0;
  virtual void visit(WhileStmt&) = 0;
  virtual void visit(PrintStmt&) = 0;
  virtual void visit(AssignStmt&) = 0;
  virtual void visit(SimpleExpr&) = 0;
  virtual void visit(IndexExpr&) = 0;
  virtual void visit(ListExpr&) = 0;
  virtual void visit(ReadExpr&) = 0;
  virtual void visit(ComplexExpr&) = 0;
  virtual void visit(SimpleBoolExpr&) = 0;
  virtual void visit(ComplexBoolExpr&) = 0;
  virtual void visit(NotBoolExpr&) = 0;
};

#endif // ABSTRACTVISITOR_H_INCLUDED
