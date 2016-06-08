#ifndef PARSER_H
#define PARSER_H

/**
 * TODO
 * - Add parentheses around expressions
**/

// Parses the token stream given by the Lexer
// It utilizes this grammar
// 	<stmts>     ::= <stmt> <stmts> | empty
// 	<stmt>      ::= <output> | <vardec> | <assign> | <cond> | <loop>
// 	<output>    ::= PRINT LPAREN <expr> RPAREN SEMICOLON | PRINTLN LPAREN <expr> RPAREN SEMICOLON
// 	<input>     ::= READINT LPAREN STRING RPAREN | READSTR LPAREN STRING RPAREN
//  <vardec>    ::= <type> <subtype> ID <vardect>
//  <vardect>   ::= ASSIGN <expr> SEMICOLON | SEMICOLON
// 	<assign>    ::= ID <listindex> ASSIGN <expr> SEMICOLON
// 	<listindex> ::= LBRACKET <expr> RBRACKET | empty
//  <type>      ::= VAR
//  <subtype>   ::= LBRACKET RBRACKET | empty
// 	<expr>      ::= <value> <exprt>
// 	<exprt>     ::= <math_rel> <expr> | empty
// 	<value>     ::= ID <listindex> | STRING | INT | BOOL | <input> | LBRACKET <exprlist> RBRACKET
//  <exprlist>  ::= <expr> <exprtail> | empty
// 	<exprtail>  ::= COMMA <expr> <exprtail> | empty
// 	<math_rel>  ::= PLUS | MINUS | DIVIDE | MULTIPLY
// 	<cond>      ::= IF <ifstmt> <condt> END
// 	<condt>     ::= ELIF <ifstmt> <condt> | ELSE <stmts> | empty
//  <ifstmt>    ::= <bexpr> THEN <stmts>
// 	<bexpr>     ::= <expr> <bexprt> | NOT <expr> <bexprt>
// 	<bexprt>    ::= <bool_rel> <expr> <bconnct> | empty
// 	<bconnct>   ::= AND <bexpr> | OR <bexpr> | empty
// 	<bool_rel>  ::= EQUAL | LESS_THAN | GREATER_THAN | LESS_THAN_EQUAL | GREATER_THAN_EQUAL | NOT_EQUAL
// 	<loop>      ::= WHILE <bexpr> DO <stmts> END

#include <memory>

#include "lexer.h"
#include "token.h"
#include "ast.h"

// Parses the token stream given by the lexer
// Example:
// 	Lexer lexer(input_file_stream);
// 	Parser parser(lexer);
// 	std::shared_ptr<StmtList> ast = Parser.parse();
class Parser
{
public:
	// Constructor
	// Takes a lexer object as the source for the token stream
	Parser(Lexer&);

	// Does the meat of the parser duties
	std::shared_ptr<StmtList> parse();

private:
	// Reference to the lexer object
	Lexer& lexer;

	// Reference to the current token
	Token cur_token;

	/// Helper functions
	// Advances the parser to the next token
	inline void advance()
	{ cur_token = lexer.next_token(); }

	// Takes an expected token from the stream.
	// Throws an error using @param msg if the token is not what was expected
	void eat(TokenType token_type, const std::string& msg);

	// Reports the error encountered
	void error(const std::string& msg);

	/// Grammar Rules
	// Applies the stmts rule
	// Expects a list of statements
	// Fills the argument with statement objects
	// Grammar Rule:
	// 	<stmts> ::= <stmt> <stmts> | empty
	void stmts(std::shared_ptr<StmtList>);

	// Applies the stmt rule
	// Expects a statement
	// Grammar Rule:
	// 	<stmt>::= <output> | <vardec> | <assign> | <cond> | <loop>
	std::shared_ptr<Stmt> stmt();

	// Applies the output rule
	// Expects some type of output
	// Grammar Rule:
	// 	<output> ::= PRINT LPAREN <expr> RPAREN SEMICOLON | PRINTLN LPAREN <expr> RPAREN SEMICOLON
	std::shared_ptr<PrintStmt> output();

	// Applies the input rule
	// Expects some sort of input
	// Grammar Rule:
	// 	<input> ::= READINT LPAREN STRING RPAREN | READSTR LPAREN STRING RPAREN
	std::shared_ptr<ReadExpr> input();

	// Applies the vardec rule
	// Expects a variable declaration
	// Grammar Rule:
  //  <vardec> ::= <type> <subtype> ID <vardect>
  std::shared_ptr<VarDecStmt> vardec();

  // Applies the vardect rule
	// Expects an assignment or semicolon
	// Grammar Rule:
  //  <vardect> ::= ASSIGN <expr> SEMICOLON | SEMICOLON
  void vardect(std::shared_ptr<VarDecStmt>);

	// Applies the assign rule
	// Expects some type of assignment
	// Grammar Rule:
	// 	<assign> ::= ID <listindex> ASSIGN <expr> SEMICOLON
	std::shared_ptr<AssignStmt> assign();

	// Applies the listindex rule
	// Expects an index for a list surrounded by brackets
	// Grammar Rule:
	// 	<listindex> ::= LBRACKET <expr> RBRACKET | empty
	std::shared_ptr<Expr> listindex();

	// Applies the type rule
	// Takes a VarDecStmt and adds the main type to it
	// Grammar Rule:
  //  <types> ::= VAR
  void type(std::shared_ptr<VarDecStmt>);

  // Applies the subtype rule
	// Takes a VarDecStmt and adds the sub type to it
	// Grammar Rule:
  //  <subtype> ::= LBRACKET RBRACKET | empty
  void subtype(std::shared_ptr<VarDecStmt>);

	// Applies the expr rule
	// Expects some type of expression
	// Grammar Rule:
	// 	<expr> ::= <value> <exprt>
	std::shared_ptr<Expr> expr();

	// Applies the exprt rule
	// Expects the end of an expression or nothing
	// Grammar Rule:
	// 	<exprt> ::= <math_rel> <expr> | empty
	std::shared_ptr<ComplexExpr> exprt(std::shared_ptr<Expr>);

	// Applies the value rule
	// Expects some type of value
	// Grammar Rule:
	// 	<value> ::= ID <listindex> | STRING | INT | BOOL | <input> | LBRACKET <exprlist> RBRACKET
	std::shared_ptr<Expr> value();

	// Applies the exprlist rule
	// Expects an
	// Grammar Rule:
	// <exprlist> ::= <expr> <exprtail> | empty
	void exprlist(std::shared_ptr<ListExpr>);

	// Applies the exprtail rule
	//
	// Grammar Rule:
	// 	<exprtail> ::= COMMA <expr> <exprtail> | empty
	void exprtail(std::shared_ptr<ListExpr>);

	// Applies the math_rel rule
	// Expects a math term
	// Returns nullptr if one is not found
	// Grammar Rule:
	// 	<math_rel> ::= PLUS | MINUS | DIVIDE | MULTIPLY
	std::shared_ptr<ComplexExpr> math_rel();

	// Applies the cond rule
	// Expects an if statement
	// Grammar Rule:
	// 	<cond> ::= IF <ifstmt> <condt> END
	std::shared_ptr<IfStmt> cond();

	// Applies the condt rule
	// Expects an ELIF or ELSE
	// Does not modify the argument otherwise
	// Grammar Rule:
	// 	<condt> ::= ELIF <ifstmt> <condt> | ELSE <stmts> | empty
	void condt(std::shared_ptr<IfStmt>);

	// Applies the ifstmt rule
	// Expects a boolean expression followed by THEN and statements
	// Grammar Rule:
	//  <ifstmt> ::= <bexpr> THEN <stmts>
	std::shared_ptr<BasicIf> ifstmt();

	// Applies the bexpr rule
	// Expects a boolean expression
	// Grammar Rule:
	// 	<bexpr> ::= <expr> <bexprt> | NOT <expr> <bexprt>
	std::shared_ptr<BoolExpr> bexpr();

	// Applies the bexprt rule
	// Expects a boolean relation
	// Returns nullptr if one is not found
	// Grammar Rule:
	// 	<bexprt> ::= <bool_rel> <expr> <bconnct> | empty
	std::shared_ptr<ComplexBoolExpr> bexprt();

	// Applies the bconnct rule
	// Expects a boolean connector
	// Returns nullptr if one is not found
	// Grammar Rule:
	// 	<bconnct> ::= AND <bexpr> | OR <bexpr> | empty
	void bconnct(std::shared_ptr<ComplexBoolExpr>);

	// Applies the bool_rel rule
	// Expects a boolean relation, but will return nullptr if one is not found
	// Grammar Rule:
	// 	<bool_rel> ::= EQUAL | LESS_THAN | GREATER_THAN | LESS_THAN_EQUAL | GREATER_THAN_EQUAL | NOT_EQUAL
	std::shared_ptr<ComplexBoolExpr> bool_rel();

	// Applies the loop rule
	// Expects a while loop
	// Grammar Rule:
	// 	<loop> ::= WHILE <bexpr> DO <stmts> END
	std::shared_ptr<WhileStmt> loop();
};

#endif // PARSER_H
