#ifndef AST_H
#define AST_H

// Declares all classes needed to construct an Abstract Syntax Tree

/**
 * It may be beneficial to add const declarations, so that Visitors
 * are prevented from modifying the tree.
 *
 * Also, make the setter functions private. Same reasoning.
 *
 * Both of the above things depend on what the Visitors are supposed to do.
 * Those are bad things to change if a Visitor is supposed to change the AST.
**/

#include <deque>
#include <string>
#include <ostream>
#include <memory>

#include "token.h"

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

#include "AbstractVisitor.h"

/// ////////////////////////////////////
/// Objects that are used as building blocks for the AST
/// ////////////////////////////////////

// The most basic of objects for the AST
// Every class here is a child of ASTNode
class ASTNode
{
public:
  // Virtual destructor
  virtual ~ASTNode() {};
  // To implement the Visitor Pattern
  virtual void accept(AbstractVisitor& visitor) = 0;
};

// A basic object for defining statements
// Every child of the head of the AST is derived from this class
class Stmt : public virtual ASTNode
{
};

// Every Stmt consists of Expr objects
class Expr : public virtual ASTNode
{
};

// Every expression that returns a boolean is derived from BoolExpr
class BoolExpr : public virtual ASTNode
{
};

// A collection of Stmt objects
// This type of object is the head of the AST
class StmtList : public virtual ASTNode
{
public:
  // Constructor
  StmtList();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Add a Stmt to this list
  void add_stmt(std::shared_ptr<Stmt> stmt)
    { stmts.push_back(stmt); }

  // Get the statement list
  std::deque<std::shared_ptr<Stmt>> get_stmts()
    { return stmts; }

private:
  // A container of Stmts
  std::deque<std::shared_ptr<Stmt>> stmts;
};

// A basic object for creating IfStmts
class BasicIf : public virtual ASTNode {
public:
  // Constructor
  BasicIf();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the boolean expression for this if statement
  void set_if_expr(std::shared_ptr<BoolExpr> expr)
    { if_expr = expr; }

  // Set the StmtList to be run if this if statement is executed
  void set_if_stmts(std::shared_ptr<StmtList> stmts)
    { if_stmts = stmts; }

  // Get the boolean expression
  std::shared_ptr<BoolExpr> get_if()
    { return if_expr; }

  // Get the statements if the boolean expression is true
  std::shared_ptr<StmtList> get_if_stmts()
    { return if_stmts; }

private:
  // Reference to the BoolExpr for this if statement
  std::shared_ptr<BoolExpr> if_expr;

  // Reference to the StmtList for this if statement
  std::shared_ptr<StmtList> if_stmts;
};

/// ////////////////////////////////////
/// Objects that inherit from Stmt
/// ////////////////////////////////////

// An entire if, then, (elseif, then)*, else?, end statement
class IfStmt : public virtual Stmt
{
public:
  // Constructor
  IfStmt();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the initial if statement
  void set_basic_if(std::shared_ptr<BasicIf> bif)
    { if_part = bif; }

  // Add else if statements
  void add_else_if(std::shared_ptr<BasicIf> bif)
    { elseifs.push_back(bif); }

  // Set the else StmtList, if it exists
  void set_else_stmts(std::shared_ptr<StmtList> stmts)
    { else_stmts = stmts; }

  // Get the initial if statement
  std::shared_ptr<BasicIf> get_if()
    { return if_part; }

  // Get the elseif if statements
  std::deque<std::shared_ptr<BasicIf>> get_elseifs()
    { return elseifs; }

  // Get the else statements
  std::shared_ptr<StmtList> get_else()
    { return else_stmts; }

private:
  // Reference to the initial if statement
  std::shared_ptr<BasicIf> if_part;

  // Container of the else if statements
  std::deque<std::shared_ptr<BasicIf>> elseifs;

  // Reference to the StmtList of the statements in the else clause
  std::shared_ptr<StmtList> else_stmts;
};

// A while statement, which houses a boolean expression and a StmtList
class WhileStmt : public virtual Stmt
{
public:
  // Constructor
  WhileStmt();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the boolean expression to check
  void set_while_expr(std::shared_ptr<BoolExpr> expr)
    { while_expr = expr; }

  // Set the statements to run in the while loop
  void set_while_stmts(std::shared_ptr<StmtList> stmts)
    { while_stmts = stmts; }

  // Get the boolean expression for this while statement
  std::shared_ptr<BoolExpr> get_while()
    { return while_expr; }

  // get the statements in this while loop
  std::shared_ptr<StmtList> get_stmts()
    { return while_stmts; }

private:
  // Reference to the boolean expression
  std::shared_ptr<BoolExpr> while_expr;

  // Reference to the StmtList
  std::shared_ptr<StmtList> while_stmts;
};

// An output statement, either print or println
class PrintStmt : public virtual Stmt
{
public:
  // Constructor
  PrintStmt();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set whether this is a println function
  void set_print_type(TokenType type)
    { print_type = type; }

  // Set the expression to print
  void set_print_expr(std::shared_ptr<Expr> expr)
    { print_expr = expr; }

  // Get the expression to print
  std::shared_ptr<Expr> get_expr()
    { return print_expr; }

  // Ge the type of print operation this is
  TokenType get_type()
    { return print_type; }

private:
  // Reference tot eh expression to print
  std::shared_ptr<Expr> print_expr;

  // Flag for whether this is a println function
  TokenType print_type;
};

// An assignment statement to a variable or element of a list
class AssignStmt : public virtual Stmt
{
public:
  // Constructor
  AssignStmt();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the token that is being assigned to
  void set_lhs_id(Token t)
    { id = t; }

  // If this is a list, set the expression for the value of the element to assign to
  void set_index_expr(std::shared_ptr<Expr> expr)
    { index_expr = expr; }

  // Set the expression that we are assigning to the left hand side
  void set_rhs_expr(std::shared_ptr<Expr> expr)
    { assign_expr = expr; }

  // Get the id to assign to
  Token get_id()
    { return id; }

  // Get the expression of the index
  std::shared_ptr<Expr> get_index()
    { return index_expr; }

  // Get the expression to assign to the id
  std::shared_ptr<Expr> get_assign()
    { return assign_expr; }

private:
  // Reference to the token to assign to
  Token id;

  // Reference to the expression that describes the value of the index to access
  std::shared_ptr<Expr> index_expr;

  // Reference to the right hand side that will be assigned to the token or element
  std::shared_ptr<Expr> assign_expr;
};

/// ////////////////////////////////////
/// Objects that inherit from Expr
/// ////////////////////////////////////

// The most basic of expressions, it is simply a number or variable or other value
class SimpleExpr : public virtual Expr
{
public:
  // Constructor
  SimpleExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the token that contains the value  for this expression
  void set_token(Token t)
    { term = t; }

  // Get the token for this simple expression
  Token get_term()
    { return term; }

private:
  // Reference to the token that holds the value (id, int, string)
  Token term;
};

// Specifies access to a specific element of a list
class IndexExpr : public virtual Expr
{
public:
  // Constructor
  IndexExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the variable that is a reference to the list that we are retrieving an element from
  void set_id(Token t)
    { id = t; }

  // Set the term that specifies the element to retrieve
  void set_index_expr(std::shared_ptr<Expr> expr)
    { index_expr = expr; }

  // Get the id of this indexed expression
  Token get_id()
    { return id; }

  // Get the index expression for this indexed expression
  std::shared_ptr<Expr> get_expr()
    { return index_expr; }

private:
  // The token of the variable that is a reference to the list
  Token id;

  // The expression that specifies the element to retrieve
  std::shared_ptr<Expr> index_expr;
};

// A list expression that stores all values of the list
class ListExpr : public virtual Expr
{
public:
  // Constructor
  ListExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Add a term to the list
  void add_expr(std::shared_ptr<Expr> expr)
    { expr_list.push_back(expr); }

  // Get the list of expressions
  std::deque<std::shared_ptr<Expr>> get_exprs()
    { return expr_list; }

private:
  // A container of the terms in the list
  std::deque<std::shared_ptr<Expr>> expr_list;
};

// An input expression, such as readint or readstr
class ReadExpr : public virtual Expr
{
public:
  // Constructor
  ReadExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the token for the value to be displayed when getting input
  void setMsg(Token m)
    { msg = m; }

  // Set the type of input this is
  void setReadType(TokenType type)
    { read_type = type; }

  // Get the message to display for this read operation
  Token get_msg()
    { return msg; }

  // Get the type of read operation this is
  TokenType get_type()
    { return read_type; }

private:
  // The token passed in as an argument to the read function
  Token msg;

  // The type of read operation this is
  TokenType read_type;
};

// A complicated mathematical expression that consists of multiple terms
class ComplexExpr : public virtual Expr
{
public:
  // Constructor
  ComplexExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the first term of this mathematical equation
  void setFirstOp(std::shared_ptr<Expr> op)
    { first_op = op; }

  // Set the mathematical operation linking the first and second terms
  void setMathRel(TokenType rel)
    { math_rel = rel; }

  // Set the right hand side of the previous mathematical operation
  void setRest(std::shared_ptr<Expr> r)
    { rest = r; }

  // Get the first operand of the expression
  std::shared_ptr<Expr> get_first_op()
    { return first_op; }

  // Get the type of the relational operator
  TokenType get_rel()
    { return math_rel; }

  // Get the rest of the expression
  std::shared_ptr<Expr> get_rest()
    { return rest; }

private:
  // Reference to the first term
  std::shared_ptr<Expr> first_op;

  // The type of mathematical operation this is signifying
  TokenType math_rel;

  // The right hand side of the mathematical operation
  std::shared_ptr<Expr> rest;
};

/// ////////////////////////////////////
/// Objects that inherit from BoolExpr
/// ////////////////////////////////////

// Contains a basic
class SimpleBoolExpr : public virtual BoolExpr
{
public:
  // Constructor
  SimpleBoolExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Sets the Expr
  void set_expr_term(std::shared_ptr<Expr> term)
    { expr_term = term; }

  // Get the currently set Expr
  std::shared_ptr<Expr> get_expr_term()
    { return expr_term; }

private:
  // Reference to the Expr
  std::shared_ptr<Expr> expr_term;
};

// A complex boolean expression that consists of a relation and possibly some boolean connections (AND, OR)
class ComplexBoolExpr : public virtual BoolExpr
{
public:
  // Constructor
  ComplexBoolExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the first operand of the complex boolean expression
  void set_first_op(std::shared_ptr<Expr> op)
    { first_op = op; }

  // Set the type of relation between the two operands
  void set_rel_type(TokenType rel)
    { bool_rel = rel; }

  // Set the second operand of the complex boolean expression
  void set_second_op(std::shared_ptr<Expr> op)
    { second_op = op; }

  // Set the boolean connection (AND, OR), if there is one
  void set_bool_con_type(TokenType con)
    { bool_con_type = con; }

  // Set what comes after the boolean connector
  void set_rest(std::shared_ptr<BoolExpr> r)
    { rest = r; }

  // Get the first operand expression
  std::shared_ptr<Expr> get_first_op()
    { return first_op; }

  // Get the boolean relation type
  TokenType get_rel()
    { return bool_rel; }

  // Get the second operand expression
  std::shared_ptr<Expr> get_second_op()
    { return second_op; }

  // Get the boolean connector type
  TokenType get_con_type()
    { return bool_con_type; }

  // Get the expression after the connector
  std::shared_ptr<BoolExpr> get_rest()
    { return rest; }

private:
  // Reference to the first operand
  std::shared_ptr<Expr> first_op;

  // The type of boolean relation this is
  TokenType bool_rel;

  // Reference to the second operand
  std::shared_ptr<Expr> second_op;

  // The type of boolean connection this is, if one is set
  TokenType bool_con_type;

  // Reference to the rest of the complex boolean expression
  std::shared_ptr<BoolExpr> rest;
};

// A not expression that negates the BoolExpr it houses
class NotBoolExpr : public virtual BoolExpr
{
public:
  // Constructor
  NotBoolExpr();

  // For the Visitor Pattern
  void accept(AbstractVisitor& visitor) override
    { visitor.visit(*this); }

  // Set the BoolExpr that this NotBoolExpr is negating
  void setBoolExpr(std::shared_ptr<BoolExpr> expr)
    { bool_expr = expr; }

  // Get the boolean expression to be negated
  std::shared_ptr<BoolExpr> get_expr()
    { return bool_expr; }

private:
  // Reference to the negated BoolExpr
  std::shared_ptr<BoolExpr> bool_expr;
};

#endif
