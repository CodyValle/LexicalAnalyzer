#include <vector>
#include <string>
#include <ostream>
#include <memory>
#include <iostream>

#include "ast.h"
#include "token.h"


using std::ostream;
using std::string;
using std::shared_ptr;
using std::endl;


// the default indentation level
const string INDENTATION = "  ";


//----------------------------------------------------------------------
// StmtList
//----------------------------------------------------------------------

void StmtList::print(ostream& out, string indent_amt)
{
  out << indent_amt << "StmtList: " << endl;
  indent_amt = indent_amt + INDENTATION;
  for (shared_ptr<Stmt> s : stmts)
    s->print(out, indent_amt);
}

void StmtList::add_stmt(shared_ptr<Stmt> stmt)
{
  stmts.push_back(stmt);
}


//----------------------------------------------------------------------
// BasicIf
//----------------------------------------------------------------------

void BasicIf::set_if_expr(shared_ptr<BoolExpr> if_expr)
{
  this->if_expr = if_expr;
}

void BasicIf::set_if_stmts(shared_ptr<StmtList> if_stmts)
{
  this->if_stmts = if_stmts;
}

shared_ptr<BoolExpr> BasicIf::get_if_expr() const
{
  return if_expr;
}

shared_ptr<StmtList> BasicIf::get_if_stmts() const
{
  return if_stmts;
}

//----------------------------------------------------------------------
// IfStmt
//----------------------------------------------------------------------

void IfStmt::print(ostream& out, string indent_amt)
{
  string INDENTATION_2 = INDENTATION + INDENTATION;
  string INDENTATION_3 = INDENTATION_2 + INDENTATION;
  out << indent_amt << "IfSmt: " << endl;
  out << indent_amt + INDENTATION << "IF: " << endl;
  if_part->get_if_expr()->print(out, indent_amt + INDENTATION_2);
  out << indent_amt + INDENTATION << "THEN: " << endl;
  if_part->get_if_stmts()->print(out, indent_amt + INDENTATION_2);
  if (elseifs.size() > 0) {
    for (unsigned int i = 0; i < elseifs.size(); i++) {
      out << indent_amt + INDENTATION << "ELSE IF: " << endl;
      shared_ptr<BoolExpr> else_if_expr = elseifs[i]->get_if_expr();
      shared_ptr<StmtList> else_if_stmts = elseifs[i]->get_if_stmts();
      else_if_expr->print(out, indent_amt + INDENTATION_3);
      out << indent_amt + INDENTATION << "ELSE IF THEN:" << endl;
      else_if_stmts->print(out, indent_amt + INDENTATION_3);
    }
  }
  if (has_else_stmts) {
    out << indent_amt + INDENTATION << "ELSE: " << endl;
    else_stmts->print(out, indent_amt + INDENTATION_2);
  }
}

void IfStmt::set_basic_if(shared_ptr<BasicIf> if_part)
{
  this->if_part = if_part;
}
void IfStmt::add_else_if(shared_ptr<BasicIf> else_if_part)
{
  elseifs.push_back(else_if_part);
}

void IfStmt::set_else_stmts(shared_ptr<StmtList> else_stmts)
{
  has_else_stmts = true;
  this->else_stmts = else_stmts;
}

//----------------------------------------------------------------------
// WhileStmt
//----------------------------------------------------------------------

void WhileStmt::print(ostream& out, string indent_amt)
{
  out << indent_amt << "WhileStmt: " << endl;
  while_expr->print(out, indent_amt + INDENTATION);
  while_stmts->print(out, indent_amt + INDENTATION);
}

void WhileStmt::set_while_expr(shared_ptr<BoolExpr> while_expr)
{
  this->while_expr = while_expr;
}

void WhileStmt::set_while_stmts(shared_ptr<StmtList> while_stmts)
{
  this->while_stmts = while_stmts;
}

//----------------------------------------------------------------------
// PrintStmt
//----------------------------------------------------------------------

void PrintStmt::print(ostream& out, string indent_amt)
{
  out << indent_amt << "PrintStmt: ";
  if (is_println)
    out << "PRINTLN" << endl;
  else
    out << "PRINT" << endl;
  print_expr->print(out, indent_amt + INDENTATION);
}

void PrintStmt::set_println(bool is_println)
{
  this->is_println = is_println;
}

void PrintStmt::set_print_expr(shared_ptr<Expr> print_expr)
{
  this->print_expr = print_expr;
}

//----------------------------------------------------------------------
// AssignStmt
//----------------------------------------------------------------------

void AssignStmt::print(ostream& out, string indent_amt)
{
  out << indent_amt << "AssignStmt: " << endl;
  string new_indent_amt = indent_amt + INDENTATION;
  if (has_list_index) {
    out << new_indent_amt << "INDEXED ID: " << id.get_lexeme() << endl;
    index_expr->print(out, new_indent_amt);
  }
  else
    out << new_indent_amt << "ID: " << id.get_lexeme() << endl;
  assign_expr->print(out, indent_amt + INDENTATION);
}

void AssignStmt::set_lhs_id(Token id)
{
  this->id = id;
}

void AssignStmt::set_list_index(bool has_list_index)
{
  this->has_list_index = has_list_index;
}

void AssignStmt::set_index_expr(shared_ptr<Expr> index_expr)
{
  this->index_expr = index_expr;
}

void AssignStmt::set_rhs_expr(shared_ptr<Expr> assign_expr)
{
  this->assign_expr = assign_expr;
}

//----------------------------------------------------------------------
// SimpleExpr
//----------------------------------------------------------------------

void SimpleExpr::print(ostream& out, string indent_amt)
{
  out << indent_amt << "SimpleExpr: ";
  if (term.get_type() == TokenType::STRING)
    out << "STRING ('" << term.get_lexeme() << "')" << endl;
  else if (term.get_type() == TokenType::INT)
    out << "INT (" << term.get_lexeme() << ")" << endl;
  else if (term.get_type() == TokenType::BOOL)
    out << "BOOL (" << term.get_lexeme() << ")" << endl;
  else if (term.get_type() == TokenType::ID)
    out << "ID (" << term.get_lexeme() << ")" << endl;
}

void SimpleExpr::set_token(Token term)
{
  this->term = term;
}

//----------------------------------------------------------------------
// IndexExpr
//----------------------------------------------------------------------

void IndexExpr::print(ostream& out, string indent_amt)
{
  out << indent_amt << "IndexExpr: " << endl;
  string new_indent_amt = indent_amt + INDENTATION;
  out << new_indent_amt << "INDEXED ID (" << id.get_lexeme() << ")" << endl;
  index_expr->print(out, new_indent_amt);
}

void IndexExpr::set_id(Token id)
{
  this->id = id;
}

void IndexExpr::set_index_expr(shared_ptr<Expr> index_expr)
{
  this->index_expr = index_expr;
}

//----------------------------------------------------------------------
// ListExpr
//----------------------------------------------------------------------

void ListExpr::print(ostream& out, string indent_amt)
{
  string INDENTATION_2 = INDENTATION + INDENTATION;
  out << indent_amt << "ListExpr: " << endl;
  for (unsigned i = 0; i < exprlist.size(); i++)
  {
    out << indent_amt + INDENTATION << "EXPR" << i << endl;
    exprlist[i]->print(out, indent_amt + INDENTATION_2);
  }
}

void ListExpr::add_expr(shared_ptr<Expr> expr)
{
  exprlist.push_back(expr);
}


//----------------------------------------------------------------------
// ReadExpr
//----------------------------------------------------------------------

void ReadExpr::print(ostream& out, string indent_amt)
{
  string read_type_str = "";
  if (is_read_str)
    read_type_str = "READSTR";
  else
    read_type_str = "READINT";
  out << indent_amt << "ReadExpr: " << read_type_str
      << " ('" << msg.get_lexeme() << "')" << endl;
}

void ReadExpr::set_msg(Token msg)
{
  this->msg = msg;
}

void ReadExpr::set_read_str(bool is_read_str)
{
  this->is_read_str = is_read_str;
}

//----------------------------------------------------------------------
// ComplexExpr
//----------------------------------------------------------------------

void ComplexExpr::print(ostream& out, string indent_amt)
{
  out << indent_amt << "ComplexExpr: " << endl;
  first_operand->print(out, indent_amt + INDENTATION);
  if (math_rel == TokenType::PLUS)
    out << indent_amt + INDENTATION << "PLUS" << endl;
  else if (math_rel == TokenType::MINUS)
    out << indent_amt + INDENTATION << "MINUS" << endl;
  else if (math_rel == TokenType::MULTIPLY)
    out << indent_amt + INDENTATION << "TIMES" << endl;
  else if (math_rel == TokenType::DIVIDE)
    out << indent_amt + INDENTATION << "DIVIDE" << endl;
  rest->print(out, indent_amt + INDENTATION);
}

void ComplexExpr::set_first_operand(shared_ptr<Expr> first_operand)
{
  this->first_operand = first_operand;
}

void ComplexExpr::set_math_rel(TokenType math_rel)
{
  this->math_rel = math_rel;
}

void ComplexExpr::set_rest(shared_ptr<Expr> rest)
{
  this->rest = rest;
}


//----------------------------------------------------------------------
// SimpleBoolExpr
//----------------------------------------------------------------------

void SimpleBoolExpr::print(ostream& out, string indent_amt)
{
  out << indent_amt << "SimpleBoolExpr:" << endl;
  expr_term->print(out, indent_amt + INDENTATION);
}

void SimpleBoolExpr::set_expr_term(shared_ptr<Expr> expr_term)
{
  this->expr_term = expr_term;
}


//----------------------------------------------------------------------
// ComplexBoolExpr
//----------------------------------------------------------------------

void ComplexBoolExpr::print(ostream& out, string indent_amt)
{
  out << indent_amt << "ComplexBoolExpr: " << endl;
  first_operand->print(out, indent_amt + INDENTATION);
  if (bool_rel == TokenType::LESS_THAN)
    out << indent_amt + INDENTATION << "LESS_THAN" << endl;
  else if (bool_rel == TokenType::LESS_THAN_EQUAL)
    out << indent_amt + INDENTATION << "LESS_THAN_EQUAL" << endl;
  else if (bool_rel == TokenType::GREATER_THAN)
    out << indent_amt + INDENTATION << "GREATER_THAN" << endl;
  else if (bool_rel == TokenType::GREATER_THAN_EQUAL)
    out << indent_amt + INDENTATION << "GREATER_THAN_EQUAL" << endl;
  else if (bool_rel == TokenType::EQUAL)
    out << indent_amt + INDENTATION << "EQUAL" << endl;
  else if (bool_rel == TokenType::NOT_EQUAL)
    out << indent_amt + INDENTATION << "NOT EQUAL" << endl;
  second_operand->print(out, indent_amt + INDENTATION);
  if (has_bool_op) {
    if (bool_connector_type == TokenType::AND)
      out << indent_amt + INDENTATION << "AND" << endl;
    else
      out << indent_amt + INDENTATION << "OR" << endl;
    rest->print(out, indent_amt + INDENTATION);
  }
}

void ComplexBoolExpr::set_first_operand(shared_ptr<Expr> first_operand)
{
  this->first_operand = first_operand;
}

void ComplexBoolExpr::set_rel_type(TokenType bool_rel)
{
  this->bool_rel = bool_rel;
}

void ComplexBoolExpr::set_second_operand(shared_ptr<Expr> second_operand)
{
  this->second_operand = second_operand;
}

void ComplexBoolExpr::set_bool_connector_type(TokenType bool_connector_type)
{
  this->bool_connector_type = bool_connector_type;
  has_bool_op = true;
}

void ComplexBoolExpr::set_rest(shared_ptr<BoolExpr> rest)
{
  this->rest = rest;
}

//----------------------------------------------------------------------
// NotBoolExpr
//----------------------------------------------------------------------

void NotBoolExpr::print(ostream& out, string indent_amt)
{
  out << indent_amt << "NotBoolExpr: " << endl;
  bool_expr->print(out, indent_amt + INDENTATION);
}

void NotBoolExpr::set_bool_expr(shared_ptr<BoolExpr> bool_expr)
{
  this->bool_expr = bool_expr;
}


