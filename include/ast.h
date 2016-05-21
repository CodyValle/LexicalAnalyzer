#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <ostream>
#include <memory>
#include "token.h"

class Expr;
class BoolExpr;

class ASTNode
{
public:
  virtual void print(std::ostream& out, std::string indent_amt) = 0;
};


class Stmt : public ASTNode
{
public:
  virtual void print(std::ostream& out, std::string indent_amt)
  { out << "Printing from Stmt class error." << std::endl; };
};


class StmtList : public ASTNode
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void add_stmt(std::shared_ptr<Stmt> stmt);
private:
  std::vector<std::shared_ptr<Stmt>> stmts;
};


class BasicIf {
public:
  void set_if_expr(std::shared_ptr<BoolExpr> if_expr);
  void set_if_stmts(std::shared_ptr<StmtList> if_stmts);
  std::shared_ptr<BoolExpr> get_if_expr() const;
  std::shared_ptr<StmtList> get_if_stmts() const;
private:
  std::shared_ptr<BoolExpr> if_expr;
  std::shared_ptr<StmtList> if_stmts;
};


class IfStmt : public Stmt
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_basic_if(std::shared_ptr<BasicIf> if_part);
  void add_else_if(std::shared_ptr<BasicIf> else_if_part);
  void set_else_stmts(std::shared_ptr<StmtList> else_stmts);
private:
  std::shared_ptr<BasicIf> if_part;
  std::vector<std::shared_ptr<BasicIf>> elseifs;
  bool has_else_stmts = false;
  std::shared_ptr<StmtList> else_stmts;
};


class WhileStmt : public Stmt
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_while_expr(std::shared_ptr<BoolExpr> while_expr);
  void set_while_stmts(std::shared_ptr<StmtList> while_stmts);
private:
  std::shared_ptr<BoolExpr> while_expr;
  std::shared_ptr<StmtList> while_stmts;
};


class PrintStmt : public Stmt
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_println(bool is_println);
  void set_print_expr(std::shared_ptr<Expr> print_expr);
private:
  std::shared_ptr<Expr> print_expr;
  bool is_println = true;
};


class AssignStmt : public Stmt
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_lhs_id(Token id);
  void set_list_index(bool has_list_index);
  void set_index_expr(std::shared_ptr<Expr> index_expr);
  void set_rhs_expr(std::shared_ptr<Expr> assign_expr);
private:
  Token id;
  bool has_list_index = false;
  std::shared_ptr<Expr> index_expr;
  std::shared_ptr<Expr> assign_expr;
};



//----------------------------------------------------------------------
// Expr
//----------------------------------------------------------------------

class Expr : public ASTNode
{
public:
  virtual void print(std::ostream& out, std::string indent_amt)
  { out << "Printing from Stmt class error." << std::endl; };
};

class BoolExpr : public Expr
{
public:
  virtual void print(std::ostream& out, std::string indent_amt) = 0;
};

class SimpleExpr : public Expr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_token(Token term);
private:
  Token term;			// plain id, int, string, or bool value
};


class IndexExpr : public Expr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_id(Token id);
  void set_index_expr(std::shared_ptr<Expr> index_expr);
private:
  Token id;
  std::shared_ptr<Expr> index_expr;
};


class ListExpr : public Expr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void add_expr(std::shared_ptr<Expr> expr);
private:
  std::vector<std::shared_ptr<Expr>> exprlist;
};


class ReadExpr : public Expr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_msg(Token msg);
  void set_read_str(bool is_read_str);
private:
  Token msg;
  bool is_read_str = true;
};


class ComplexExpr : public Expr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_first_operand(std::shared_ptr<Expr> first_operand);
  void set_math_rel(TokenType math_rel);
  void set_rest(std::shared_ptr<Expr> rest);
private:
  std::shared_ptr<Expr> first_operand;
  TokenType math_rel;
  std::shared_ptr<Expr> rest;
};


class SimpleBoolExpr : public BoolExpr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_expr_term(std::shared_ptr<Expr> expr_term);
private:
  std::shared_ptr<Expr> expr_term;
};


class ComplexBoolExpr : public BoolExpr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_first_operand(std::shared_ptr<Expr> first_operand);
  void set_rel_type(TokenType bool_rel);
  void set_second_operand(std::shared_ptr<Expr> second_operand);
  void set_bool_connector_type(TokenType bool_connector_type);
  void set_rest(std::shared_ptr<BoolExpr> rest);
private:
  std::shared_ptr<Expr> first_operand;
  TokenType bool_rel;
  std::shared_ptr<Expr> second_operand;
  bool has_bool_op = false;
  TokenType bool_connector_type;
  std::shared_ptr<BoolExpr> rest;
};


class NotBoolExpr : public BoolExpr
{
public:
  void print(std::ostream& out, std::string indent_amt);
  void set_bool_expr(std::shared_ptr<BoolExpr> bool_expr);
private:
  std::shared_ptr<BoolExpr> bool_expr;
};

#endif
