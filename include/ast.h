#ifndef AST_H
#define AST_H

// Declares all types needed to construct an Abstract Syntax Tree

#include <deque>
#include <string>
#include <ostream>
#include <memory>

#include "token.h"

/// ////////////////////////////////////
/// Objects that are used as building blocks for the AST
/// ////////////////////////////////////

// The most basic of objects for the AST
// Every class here is a child of ASTNode
class ASTNode
{
public:
  // Every node in the AST needs to use this to be printed
  virtual void print(std::ostream&, std::string) = 0;
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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Add a Stmt to this list
  inline void add_stmt(std::shared_ptr<Stmt> stmt)
    { stmts.push_back(stmt); };

private:
  // A container of Stsmts
  std::deque<std::shared_ptr<Stmt>> stmts;
};

// A basic object for creating IfStmts
class BasicIf : public virtual ASTNode {
public:
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the boolean expression for this if statement
  inline void set_if_expr(std::shared_ptr<BoolExpr> expr)
    { if_expr = expr; };

  // Set the StmtList to be run if this if statement is executed
  inline void set_if_stmts(std::shared_ptr<StmtList> stmts)
    { if_stmts = stmts; };

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the initial if statement
  inline void set_basic_if(std::shared_ptr<BasicIf> bif)
    { if_part = bif; };

  // Add else if statements
  inline void add_else_if(std::shared_ptr<BasicIf> bif)
    { elseifs.push_back(bif); };

  // Set the else StmtList, if it exists
  inline void set_else_stmts(std::shared_ptr<StmtList> stmts)
    { else_stmts = stmts; };

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the boolean expression to check
  inline void set_while_expr(std::shared_ptr<BoolExpr> expr)
    { while_expr = expr; };

  // Set the statements to run in the while loop
  inline void set_while_stmts(std::shared_ptr<StmtList> stmts)
    { while_stmts = stmts; };

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

  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set whether this is a println function
  inline void set_print_type(TokenType type)
    { print_type = type; };

  // Set the expression to print
  inline void set_print_type(std::shared_ptr<Expr> expr)
    { print_expr = expr; };

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the token that is being assigned to
  inline void set_lhs_id(Token t)
    { id = t; };

  // If this is a list, set the expression for the value of the element to assign to
  inline void set_index_expr(std::shared_ptr<Expr> expr)
    { index_expr = expr; };

  // Set the expression that we are assigning to the left hand side
  inline void set_rhs_expr(std::shared_ptr<Expr> expr)
    { assign_expr = expr; };

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the token that contains the value  for this expression
  inline void set_token(Token t)
    { term = t; };

private:
  // Reference to the token that holds the value (id, int, string)
  Token term;
};

// Specifies access to a specific element of a list
class IndexExpr : public virtual Expr
{
public:
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the variable that is a reference to the list that we are retrieving an element from
  inline void set_id(Token t)
    { id = t; };

  // Set the term that specifies the element to retrieve
  inline void set_index_expr(std::shared_ptr<Expr> expr)
    { index_expr = expr; };

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Add a term to the list
  inline void add_expr(std::shared_ptr<Expr> expr)
    { expr_list.push_back(expr); };

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

  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the token for the value to be displayed when getting input
  inline void setMsg(Token m)
    { msg = m; };

  // Set the type of input this is
  inline void setReadType(TokenType type)
    { read_type = type; };

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the first term of this mathematical equation
  inline void setFirstOp(std::shared_ptr<Expr> op)
    { first_op = op; };

  // Set the mathematical operation linking the first and second terms
  inline void setMathRel(TokenType rel)
    { math_rel = rel; };

  // Set the right hand side of the previous mathematical operation
  inline void setRest(std::shared_ptr<Expr> r)
    { rest = r; };

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Sets the Expr
  inline void set_expr_term(std::shared_ptr<Expr> term)
    { expr_term = term; };

  // Get the currently set Expr
  inline std::shared_ptr<Expr> get_expr_term()
    { return expr_term; };

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

  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the first operand of the complex boolean expression
  inline void set_first_op(std::shared_ptr<Expr> op)
    { first_op = op; };

  // Set the type of relation between the two operands
  inline void set_rel_type(TokenType rel)
    { bool_rel = rel; };

  // Set the second operand of the complex boolean expression
  inline void set_second_op(std::shared_ptr<Expr> op)
    { second_op = op; };

  // Set the boolean connection (AND, OR), if there is one
  inline void set_bool_con_type(TokenType con)
    { bool_con_type = con; };

  // Set what comes after the boolean connector
  inline void set_rest(std::shared_ptr<BoolExpr> r)
    { rest = r; }

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
  // Inherited print function from ASTNode
  void print(std::ostream&, std::string);

  // Set the BoolExpr that this NotBoolExpr is negating
  inline void setBoolExpr(std::shared_ptr<BoolExpr> expr)
    { bool_expr = expr; };

private:
  // Reference to the negated BoolExpr
  std::shared_ptr<BoolExpr> bool_expr;
};

#endif
