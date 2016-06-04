// Defines all necessary functions for all the nodes of the AST

#include <iostream>
#include "ast.h"

//----------------------------------------------------------------------
// StmtList
//----------------------------------------------------------------------

// StmtList constructor
StmtList::StmtList() :
  stmts(0)
{}

//----------------------------------------------------------------------
// BasicIf
//----------------------------------------------------------------------

// BasicIf constructor
BasicIf::BasicIf() :
  if_expr(nullptr),
  if_stmts(nullptr)
{}

//----------------------------------------------------------------------
// IfStmt
//----------------------------------------------------------------------

// IfStmt constructor
IfStmt::IfStmt() :
  if_part(nullptr),
  elseifs(0),
  else_stmts(nullptr)
{}

//----------------------------------------------------------------------
// WhileStmt
//----------------------------------------------------------------------

// WhileStmt constructor
WhileStmt::WhileStmt() :
  while_expr(nullptr),
  while_stmts(nullptr)
{}

//----------------------------------------------------------------------
// PrintStmt
//----------------------------------------------------------------------

// PrintStmt constructor
PrintStmt::PrintStmt() :
  print_expr(nullptr),
  print_type(TokenType::PRINTLN)  // Default to PRINTLN
{}

//----------------------------------------------------------------------
// VarDecStmt
//----------------------------------------------------------------------

// VarDecStmt constructor
VarDecStmt::VarDecStmt() :
  id(Token()),
  type(TokenType::UNKNOWN),
  sub_type(TokenType::UNKNOWN),
  assign_expr(nullptr)
{}

//----------------------------------------------------------------------
// AssignStmt
//----------------------------------------------------------------------

// AssignStmt constructor
AssignStmt::AssignStmt() :
  //type(TokenType::UNKNOWN),
  id(Token()),
  index_expr(nullptr),
  assign_expr(nullptr)
{}

//----------------------------------------------------------------------
// SimpleExpr
//----------------------------------------------------------------------

// SimpleExpr constructor
SimpleExpr::SimpleExpr() :
  term(Token())
{}

//----------------------------------------------------------------------
// IndexExpr
//----------------------------------------------------------------------

// IndexExpr constructor
IndexExpr::IndexExpr() :
  id(Token()),
  index_expr(nullptr)
{}

//----------------------------------------------------------------------
// ListExpr
//----------------------------------------------------------------------

// ListExpr constructor
ListExpr::ListExpr() :
  expr_list(0)
{}

//----------------------------------------------------------------------
// ReadExpr
//----------------------------------------------------------------------

// ReadExpr constructor
ReadExpr::ReadExpr() :
  msg(Token()),
  read_type(TokenType::READSTR) // Default to READSTR
{}

//----------------------------------------------------------------------
// ComplexExpr
//----------------------------------------------------------------------

// ComplexExpr constructor
ComplexExpr::ComplexExpr() :
  first_op(nullptr),
  math_rel(TokenType::UNKNOWN),
  rest(nullptr)
{}

//----------------------------------------------------------------------
// SimpleBoolExpr
//----------------------------------------------------------------------

// SimpleBoolExpr constructor
SimpleBoolExpr::SimpleBoolExpr() :
  expr_term(nullptr)
{}

//----------------------------------------------------------------------
// ComplexBoolExpr
//----------------------------------------------------------------------

// ComplexBoolExpr constructor
ComplexBoolExpr::ComplexBoolExpr() :
  first_op(nullptr),
  bool_rel(TokenType::UNKNOWN),
  second_op(nullptr),
  bool_con_type(TokenType::UNKNOWN),
  rest(nullptr)
{}

//----------------------------------------------------------------------
// NotBoolExpr
//----------------------------------------------------------------------

// NotBoolExpr constructor
NotBoolExpr::NotBoolExpr() :
  bool_expr(nullptr)
{}
