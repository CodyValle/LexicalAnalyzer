// The Parser object
// Contains definitions for everything in parser.h

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "token.h"
#include "parser.h"
#include "lexer.h"
#include "exception.h"

// Parser constructor definition
Parser::Parser(Lexer& lexer) :
	lexer(lexer), current_token(TokenType::EOS, "", 0, 0)
{
}

// Parser eat definition
void Parser::eat(TokenType token_type, const std::string& msg)
{
	if (token_type == current_token.get_type())
		advance();
	else
		error(msg);
}

// Parser error definition
void Parser::error(const std::string& msg)
{
	std::ostringstream error_str;
	error_str << msg << ", found " << current_token.get_type() << "{" << current_token.get_lexeme() << "}.";
	int line = current_token.get_line();
	int column = current_token.get_column();
	throw Exception(error_str.str(), line, column, ExceptionType::PARSER);
}

// Parser parse definition
void Parser::parse()
{
	advance();
	stmts();
	eat(TokenType::EOS, "expecting end-of-file");
}

// Pasrser stmts definition
// Grammar Rule:
// 	<stmts> ::= <stmt> <stmts> | empty
void Parser::stmts()
{
	// Check the base case
	switch (current_token.get_type())
	{
	case TokenType::EOS:
	case TokenType::END:
	case TokenType::ELIF:
  case TokenType::ELSE:
		return;

	default: break;
	}

	// Apply the rule
	stmt();
	stmts();
}

// Parser stmt definition
// Grammar Rule":
// 	<stmt>::= <output> | <assign> | <cond> | <loop>
void Parser::stmt()
{
	// What kind of statement is this?
	switch (current_token.get_type())
	{
	case TokenType::PRINT:
	case TokenType::PRINTLN:
		output();
		break;

	case TokenType::ID:
		assign();
		break;

	case TokenType::IF:
		cond();
		break;

	case TokenType::WHILE:
		loop();
		break;

	default:
		error("expected output, assignment, conditional, or loop statement. You must assign the readstr and readint function return values to an identifier");
		break;
	}
}

// Parser output definition
// Grammar Rule:
// 	<output> ::= PRINT LPAREN <expr> RPAREN SEMICOLON | PRINTLN LPAREN <expr> RPAREN SEMICOLON
void Parser::output()
{
	switch (current_token.get_type())
	{
	case TokenType::PRINT:
		eat(TokenType::PRINT, "expected print statement");
		break;

	case TokenType::PRINTLN:
		eat(TokenType::PRINTLN, "expected print line statement");
		break;

	default:
		error("internal error. No idea what caused it");
		break;
	}

	// Both rules end with this
	eat(TokenType::LPAREN, "expected left parenthesis");
	expr();
	eat(TokenType::RPAREN, "expected right parenthesis");
	eat(TokenType::SEMICOLON, "expected semicolon");
}

// Parser bexprt definition
// Grammar Rule:
// 	<input> ::= READINT LPAREN <string> RPAREN | READSTR LPAREN <string> RPAREN
void Parser::input()
{
	switch (current_token.get_type())
	{
	case TokenType::READINT:
		eat(TokenType::READINT, "expected readint function");
		break;

	case TokenType::READSTR:
		eat(TokenType::READSTR, "expected readstr function");
		break;

	default:
		error("expected an input of some sort (readint, readstr)");
		break;
	}

	// Both rules end with this
	eat(TokenType::LPAREN, "expected left parenthesis");
	eat(TokenType::STRING, "expected string");
	eat(TokenType::RPAREN, "expected right parenthesis");
}

// Parser assign definition
// Grammar Rule:
// 	<assign> ::= ID <listindex> ASSIGN <expr> SEMICOLON
void Parser::assign()
{
	eat(TokenType::ID, "expecting identifier for assignment statement");
	listindex();
	eat(TokenType::ASSIGN, "expecting assignment operator");
	expr();
	eat(TokenType::SEMICOLON, "expecting semicolon");
}

// Parser listindex definition
// Grammar Rule:
// 	<listindex> ::= LBRACKET <expr> RBRACKET | empty
void Parser::listindex()
{
	if (current_token.get_type() != TokenType::LBRACKET) return;

	eat(TokenType::LBRACKET, "expected left bracket");
	expr();
	eat(TokenType::RBRACKET, "expected right bracket");
}

// Parser expr definition
// Grammar Rule:
// 	<expr> ::= <value> <exprt>
void Parser::expr()
{
	value();
	exprt();
}

// Parser exprt definition
// Grammar Rule:
// 	<exprt> ::= <math_rel> <expr> | <bool_rel> <expr> | empty
void Parser::exprt()
{
	if (math_rel())
		expr();
	else if (bool_rel())
		expr();
}

// Parser value definition
// Grammar Rule:
// 	<value> ::= ID <listindex> | STRING | INT | BOOL | <input> | LBRACKET <exprlist> RBRACKET
void Parser::value()
{
	switch (current_token.get_type())
	{
	case TokenType::ID:
		eat(TokenType::ID, "expected identifier");
		listindex();
		break;

	case TokenType::STRING:
		eat(TokenType::STRING, "expected string");
		break;

	case TokenType::INT:
		eat(TokenType::INT, "expected integer");
		break;

	case TokenType::TRUE:
		eat(TokenType::TRUE, "expected true keyword");
		break;

	case TokenType::FALSE:
		eat(TokenType::FALSE, "expected false keyword");
		break;

	case TokenType::LBRACKET:
		eat(TokenType::LBRACKET, "expected left bracket");
		exprlist();
		eat(TokenType::RBRACKET, "expected right bracket");
		break;

	case TokenType::COMMA:
		error("expected a value before the comma");

	default:
		input();
		break;
	}
}

// Parser bexprt definition
// Grammar Rule:
// <exprlist> ::= <expr> <exprtail> | empty
void Parser::exprlist()
{
	if (current_token.get_type() == TokenType::RBRACKET) return;
	expr();
	exprtail();
}

// Parser bexprt definition
// Grammar Rule:
// 	<exprtail> ::= COMMA <expr> <exprtail> | empty
void Parser::exprtail()
{
	if (current_token.get_type() == TokenType::COMMA)
	{
		eat(TokenType::COMMA, "expected comma between expressions");
		expr();
		exprtail();
	}
}

// Parser bexprt definition
// Grammar Rule:
// 	<math_rel> ::= PLUS | MINUS | DIVIDE | MULTIPLY
bool Parser::math_rel()
{
	switch (current_token.get_type())
	{
	case TokenType::PLUS:
		eat(TokenType::PLUS, "expected plus operator");
		return true;

	case TokenType::MINUS:
		eat(TokenType::MINUS, "expected minus operator");
		return true;

	case TokenType::DIVIDE:
		eat(TokenType::DIVIDE, "expected division operator");
		return true;

	case TokenType::MULTIPLY:
		eat(TokenType::MULTIPLY, "expected multiplication operator");
		return true;

	default: break;
	}

	return false;
}

// Parser cond definition
// Grammar Rule:
// 	<cond> ::= IF <bexpr> THEN <stmts> <condt> END
void Parser::cond()
{
	eat(TokenType::IF, "expected if keyword");
	bexpr();
	eat(TokenType::THEN, "expected then keyword");
	stmts();
	condt();
	eat(TokenType::END, "expected end keyword");
}

// Parser condt definition
// Grammar Rule:
// 	<condt> ::= ELIF <bexpr> THEN <stmts> <condt> | ELSE <stmts> | empty
void Parser::condt()
{
	switch (current_token.get_type())
	{
	case TokenType::ELIF:
		eat(TokenType::ELIF, "expected elif keyword");
		bexpr();
		eat(TokenType::THEN, "expected then keyword");
		stmts();
		condt();
		break;

	case TokenType::ELSE:
		eat(TokenType::ELSE, "expected else keyword");
		stmts();
		break;

	default: break;
	}
}

// Parser bexpr definition
// Grammar Rule:
// 	<bexpr> ::= <expr> <bexprt> | NOT <expr> <bexprt>
void Parser::bexpr()
{
	if (current_token.get_type() == TokenType::NOT)
		eat(TokenType::NOT, "expected a not boolean operator");

	expr();
	bexprt();
}

// Parser bexprt definition
// Grammar Rule:
// 	<bexprt> ::= <bool_rel> <expr> <bconnct> | empty
void Parser::bexprt()
{
	if (bool_rel())
	{
		expr();
		bconnct();
	}
}

// Parser bconnct definition
// Grammar Rule:
// 	<bconnct> ::= AND <bexpr> | OR <bexpr> | empty
void Parser::bconnct()
{
	switch (current_token.get_type())
	{
	case TokenType::AND:
		eat(TokenType::AND, "expected and boolean connector");
		break;

	case TokenType::OR:
		eat(TokenType::OR, "expected or boolean connector");
		break;

		default: break;
	}
}

// Parser bool_rel definition
// Grammar Rule:
// 	<bool_rel> ::= EQUAL | LESS_THAN | GREATER_THAN | LESS_THAN_EQUAL | GREATER_THAN_EQUAL | NOT_EQUAL
bool Parser::bool_rel()
{
	switch (current_token.get_type())
	{
	case TokenType::EQUAL:
		eat(TokenType::EQUAL, "expected equality comparison operator");
		return true;

	case TokenType::LESS_THAN:
		eat(TokenType::LESS_THAN, "expected less than comparison operator");
		return true;

	case TokenType::GREATER_THAN:
		eat(TokenType::GREATER_THAN, "expected greater than comparison operator");
		return true;

	case TokenType::LESS_THAN_EQUAL:
		eat(TokenType::LESS_THAN_EQUAL, "expected less than or equal to comparison operator");
		return true;

	case TokenType::GREATER_THAN_EQUAL:
		eat(TokenType::GREATER_THAN_EQUAL, "expected greater than or equal to comparison operator");
		return true;

	case TokenType::NOT_EQUAL:
		eat(TokenType::NOT_EQUAL, "expected not equal comparison operator");
		return true;

	default: break;
	}

	return false;
}

// Parser loop definition
// Grammar Rule:
// 	<loop> ::= WHILE <bexpr> DO <stmts> END
void Parser::loop()
{
	eat(TokenType::WHILE, "expected while keyword");
	bexpr();
	eat(TokenType::DO, "expected do keyword");
	stmts();
	eat(TokenType::END, "expected end keyword");
}
