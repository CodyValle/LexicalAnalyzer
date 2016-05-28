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

// Preprocessor macro to simplify making shared_ptr's
// Usage example:
//  SP(Expr, sptr); // Expands to std::shared_ptr<Expr> sptr = std::make_shared<Expr>();
#define SP(type, name) std::shared_ptr<type> name = std::make_shared<type>()

// Parser constructor (lexer) definition
Parser::Parser(Lexer& lexer) :
	lexer(lexer),
	cur_token(TokenType::EOS, "", 0, 0)
{
}

// Parser eat definition
void Parser::eat(TokenType token_type, const std::string& msg)
{
	// Consume the expected TokenType
	if (token_type == cur_token.get_type())
		advance();

	// Otherwise throw the error
	else
		error(msg);
}

// Parser error definition
void Parser::error(const std::string& msg)
{
	// Create the error message
	std::ostringstream error_str;
	error_str << msg << ", found " << cur_token.get_type() << "{" << cur_token.get_lexeme() << "}.";

	// Throw the error
	throw Exception(error_str.str(),
									cur_token.get_line(),
									cur_token.get_column(),
									ExceptionType::PARSER);
}

// Parser parse definition
std::shared_ptr<StmtList> Parser::parse()
{
	// Load the first token
	advance();

	// Create the StmtList object and fill it
	SP(StmtList, ret);
	stmts(ret);

	// Consume the End-Of-Stream token
	eat(TokenType::EOS, "expecting end-of-file");

	// Return the top StmtList object
	return ret;
}

/// Grammar Rules
// Pasrser stmts definition
// Grammar Rule:
// 	<stmts> ::= <stmt> <stmts> | empty
void Parser::stmts(std::shared_ptr<StmtList> ret)
{
	// Check the base cases
	switch (cur_token.get_type())
	{
	case TokenType::EOS: // Done reading file
	case TokenType::END: // Done reading an if/elif/else/while statement
	case TokenType::ELIF: // Done reading an if/elif statement
  case TokenType::ELSE: // Done reading an if/elif statement
		return;

	default: break;
	}

	// Apply the rule
	std::shared_ptr<Stmt> st = stmt();

	// Add the statement if it exists (it always should, or an error will be thrown before we get here)
	if (st) ret->add_stmt(st);

	// Check for more statements
	stmts(ret);
}

// Parser stmt definition
// Grammar Rule":
// 	<stmt> ::= <output> | <assign> | <cond> | <loop>
std::shared_ptr<Stmt> Parser::stmt()
{
	// What kind of statement is this?
	switch (cur_token.get_type())
	{
		// An output statement
	case TokenType::PRINT:
	case TokenType::PRINTLN:
		return output();

		// An assignment statement
	case TokenType::ID:
		return assign();

		// An if statement
	case TokenType::IF:
		return cond();

		// A while loop
	case TokenType::WHILE:
		return loop();

		// Unknown, syntax error
	default:
		error("expected output, assignment, conditional, or loop statement. You must assign the readstr and readint function return values to an identifier");
		break;
	}

	// Unreachable code due to a call to error in the default case
	return nullptr;
}

// Parser output definition
// Grammar Rule:
// 	<output> ::= PRINT LPAREN <expr> RPAREN SEMICOLON | PRINTLN LPAREN <expr> RPAREN SEMICOLON
std::shared_ptr<PrintStmt> Parser::output()
{
	// Create the return object
	SP(PrintStmt, ret);

	// What type of output is this?
	switch (cur_token.get_type())
	{
		// Normal print call
	case TokenType::PRINT:
		ret->set_print_type(TokenType::PRINT);
		// We want to fall through to the PRINTLN case

		// println call
	case TokenType::PRINTLN:
		advance();
		break;

		// Should never be reached, since output is only called when the current token's type is PRINT or PRINTLN
	default:
		error("expected PRINT or PRINTLN");
		break;
	}

	// Both rules end with this
	eat(TokenType::LPAREN, "expected left parenthesis '('");
	ret->set_print_type(expr());
	eat(TokenType::RPAREN, "expected right parenthesis ')'");
	eat(TokenType::SEMICOLON, "expected semicolon ';'");

	return ret;
}

// Parser input definition
// Grammar Rule:
// 	<input> ::= READINT LPAREN STRING RPAREN | READSTR LPAREN STRING RPAREN
std::shared_ptr<ReadExpr> Parser::input()
{
	// Create the return object
	SP(ReadExpr, ret);

	// What type of token is this?
	switch (cur_token.get_type())
	{
		// We are reading an integer
	case TokenType::READINT:
		// Set the type of read operation this is
		ret->setReadType(TokenType::READINT);

		// We are reading a string
	case TokenType::READSTR:
		advance();

		// A ReadExpr defaults to a READSTR function, no need to set_read_type to READSTR
		break;

		// Should never be reached, since input is only called when the current token's type is READINT or READSTR
	default:
		error("expected READINT or READSTR");
	}

	// Both rules end with this
	eat(TokenType::LPAREN, "expected left parenthesis '('");

	// Add the current string token to the ReadExpr
	ret->setMsg(cur_token);
	eat(TokenType::STRING, "expected string");

	eat(TokenType::RPAREN, "expected right parenthesis ')'");

	// Return the ReadExpr
	return ret;
}

// Parser assign definition
// Grammar Rule:
// 	<assign> ::= ID <listindex> ASSIGN <expr> SEMICOLON
std::shared_ptr<AssignStmt> Parser::assign()
{
	// Create return object
	SP(AssignStmt, ret);

	// Set the ID for this assign statement
	ret->set_lhs_id(cur_token);
	eat(TokenType::ID, "expecting identifier for assignment statement");

	// If the ID is for a list, we may be assigning to an index
	std::shared_ptr<Expr> list = listindex();
	if (list)
	{
		// We are assigning to an index
		ret->set_index_expr(list);
	}

	// End the rule
	eat(TokenType::ASSIGN, "expecting assignment operator '='");
	ret->set_rhs_expr(expr());
	eat(TokenType::SEMICOLON, "expecting semicolon ';'");

	return ret;
}

// Parser listindex definition
// Grammar Rule:
// 	<listindex> ::= LBRACKET <expr> RBRACKET | empty
std::shared_ptr<Expr> Parser::listindex()
{
	// Check if this rule should be applied
	if (cur_token.get_type() != TokenType::LBRACKET) return nullptr;
	advance();

	// Add the expression between the brackets
	std::shared_ptr<Expr> ret = expr();

	// Close the listindex
	eat(TokenType::RBRACKET, "expected right bracket ']'");

	return ret;
}

// Parser expr definition
// Grammar Rule:
// 	<expr> ::= <value> <exprt>
std::shared_ptr<Expr> Parser::expr()
{
	// Expressions start with values, so that will be our return object
	std::shared_ptr<Expr> ret = value();

	std::shared_ptr<Expr> tail = exprt(ret);

	return tail ? tail : ret;
}

// Parser exprt definition
// Grammar Rule:
// 	<exprt> ::= <math_rel> <expr> | empty
std::shared_ptr<ComplexExpr> Parser::exprt(std::shared_ptr<Expr> ret)
{
	// Check if this exprt is part of a mathematical equation
	std::shared_ptr<ComplexExpr> math = math_rel();

	if (math)
	{
		// This is a mathematical equation
		// Add the passed in first operand
		math->setFirstOp(ret);

		// Set the rest of the equation
		math->setRest(expr());

		// Return the ComplexExpr
		return math;
	}

	// Return the nullptr
	return nullptr;
}

// Parser value definition
// Grammar Rule:
// 	<value> ::= ID <listindex> | STRING | INT | BOOL | <input> | LBRACKET <exprlist> RBRACKET
std::shared_ptr<Expr> Parser::value()
{
	// What type of value is this?
	switch (cur_token.get_type())
	{
		// Variable's value
	case TokenType::ID:
	{
		// Save token for later
		Token tok = cur_token;

		// Consume the ID token
		advance();

		// Is this variable a list?
		std::shared_ptr<Expr> li = listindex();
		if (li)
		{
			// It is an indexed variable
			// Create an IndexExpr and add the saved token
			SP(IndexExpr, ret);
			ret->set_id(tok);

			// Add the ListExpr to the IndexExpr and return it
			ret->set_index_expr(li);
			return ret;
		}
		else
		{
			// It is not a list
			// Just a simple expression
			SP(SimpleExpr, ret);
			ret->set_token(tok);
			return ret;
		}
	}

		// Simple expression. Pretty self-explanatory
	case TokenType::STRING:
	case TokenType::INT:
	case TokenType::BOOL:
	{
		SP(SimpleExpr, ret);
		ret->set_token(cur_token);
		advance();
		return ret;
	}

		// List type value
	case TokenType::LBRACKET:
	{
		advance();

		// Create the ListExpr object
		SP(ListExpr, ret);

		// Fill it with expressions
		exprlist(ret);

		// Wrap up the rule
		eat(TokenType::RBRACKET, "expected right bracket ']'");
		return ret;
	}

		// Input from the terminal value
	case TokenType::READINT:
	case TokenType::READSTR:
		return input();
		break;

		// A comma is not a value
	case TokenType::COMMA:
		error("expected a value before the comma");

		// Error. Unexpected token encountered
	default:
		error("expected a value (integer, string, boolean, etc.)");
		break;
	}

	// Unreachable code due to a call to error in the default case
	return nullptr;
}

// Parser exprlist definition
// Grammar Rule:
// <exprlist> ::= <expr> <exprtail> | empty
void Parser::exprlist(std::shared_ptr<ListExpr> ret)
{
	// exprlist is only called by value when a list type value is encountered
	// If this statement is true, then the list is empty
	if (cur_token.get_type() == TokenType::RBRACKET) return;

	// Add the first expression to the ListExpr
	ret->add_expr(expr());

	// Add the rest of the expressions to the ListExpr
	exprtail(ret);
}

// Parser exprtail definition
// Grammar Rule:
// 	<exprtail> ::= COMMA <expr> <exprtail> | empty
void Parser::exprtail(std::shared_ptr<ListExpr> ret)
{
	// If there isn't a comma, there are no more expressions
	if (cur_token.get_type() == TokenType::COMMA)
	{
		advance();

		// Add this expression
		ret->add_expr(expr());

		// Add the next expression
		exprtail(ret);
	}
}

// Parser math_rel definition
// Grammar Rule:
// 	<math_rel> ::= PLUS | MINUS | DIVIDE | MULTIPLY
std::shared_ptr<ComplexExpr> Parser::math_rel()
{
	//
	switch (cur_token.get_type())
	{
	case TokenType::PLUS:
	case TokenType::MINUS:
	case TokenType::DIVIDE:
	case TokenType::MULTIPLY:
	{
		SP(ComplexExpr, ret);
		ret->setMathRel(cur_token.get_type());
		advance();
		return ret;
	}

	default: break;
	}

	return nullptr;
}

// Parser cond definition
// Grammar Rule:
// 	<cond> ::= IF <ifstmt> <condt> END
std::shared_ptr<IfStmt> Parser::cond()
{
	// Create the IfStmt
	SP(IfStmt, ret);

	// Consume the appropriate token
	eat(TokenType::IF, "expected if keyword");

	// Add the BasicIf to the IfStmt
	ret->set_basic_if(ifstmt());

	// Add any elifs or an else, if any exist
	condt(ret);

	// Consume the appropriate token
	eat(TokenType::END, "expected end keyword");

	// Return the IfStmt
	return ret;
}

// Parser condt definition
// Grammar Rule:
// 	<condt> ::= ELIF <ifstmt> <condt> | ELSE <stmts> | empty
void Parser::condt(std::shared_ptr<IfStmt> ret)
{

	switch (cur_token.get_type())
	{
	case TokenType::ELIF:
	{
		advance();

		// Add the BasicIf to the IfStmt
		ret->add_else_if(ifstmt());

		// Add any elifs or an else, if any exist
		condt(ret);

		break;
	}

	case TokenType::ELSE:
	{
		advance();

		// Get all the else statements, add them to the passed in IfStmt
		SP(StmtList, stmtsp);
		stmts(stmtsp);
		ret->set_else_stmts(stmtsp);
	}

	default: break;
	}
}

// Parser condt definition
// Grammar Rule:
// 	<ifstmt> ::= <bexpr> THEN <stmts>
std::shared_ptr<BasicIf> Parser::ifstmt()
{
	// Start with a BasicIf
	SP(BasicIf, bif);

	// Add the boolean expression to it
	bif->set_if_expr(bexpr());

	// Consume the appropriate token
	eat(TokenType::THEN, "expected then keyword");

	// Get the statements for the BasicIf
	SP(StmtList, stmtsp);
	stmts(stmtsp);
	bif->set_if_stmts(stmtsp);

	return bif;
}

// Helper function for bexpr
// Wraps the passed in BoolExpr in a NotBoolExpr
std::shared_ptr<BoolExpr> notWrapper(std::shared_ptr<BoolExpr> in)
{
	// This is actually a NotBoolExpr, so create it, add the normal expression, and return the NotBoolExpr
	SP(NotBoolExpr, nbexp);
	nbexp->setBoolExpr(in);
	return nbexp;
}

// Parser bexpr definition
// Grammar Rule:
// 	<bexpr> ::= <expr> <bexprt> | NOT <expr> <bexprt>
std::shared_ptr<BoolExpr> Parser::bexpr()
{
	// Flag for if this is a NotBoolExpr or not
	bool notExpr = false;
	if (cur_token.get_type() == TokenType::NOT)
	{
		// This is a NotBoolExpr
		notExpr = true;
		advance();
	}

	// Get the expression
	SP(SimpleBoolExpr, ret);
	ret->set_expr_term(expr());

	// Check if this is a ComplexBoolExpr
	std::shared_ptr<ComplexBoolExpr> tail = bexprt();
	if (tail)
	{
		// This is a ComplexBoolExpr
		// Set the first operand
		tail->set_first_op(ret->get_expr_term());

		// Return the object
		return notExpr ? notWrapper(tail) : tail;
	}

	// Regular expression without NOT
	return notExpr ? notWrapper(ret) : ret;
}

// Parser bexprt definition
// Grammar Rule:
// 	<bexprt> ::= <bool_rel> <expr> <bconnct> | empty
std::shared_ptr<ComplexBoolExpr> Parser::bexprt()
{
	// Is this a boolean relation?
	std::shared_ptr<ComplexBoolExpr> ret = bool_rel();
	if (ret)
	{
		// It is, get the expression for it
		ret->set_second_op(expr());

		// If there is a boolean connector, bconnct will modify ret appropriately.
		bconnct(ret);
	}

	return ret;
}

// Parser bconnct definition
// Grammar Rule:
// 	<bconnct> ::= AND <bexpr> | OR <bexpr> | empty
void Parser::bconnct(std::shared_ptr<ComplexBoolExpr> ret)
{
	switch (cur_token.get_type())
	{
	case TokenType::AND:
	case TokenType::OR:
		// Set the boolean connection type
		ret->set_bool_con_type(cur_token.get_type());
		advance();

		// Set the rest of the complex boolean expression
		ret->set_rest(bexpr());

	default: break;
	}
}

// Parser bool_rel definition
// Grammar Rule:
// 	<bool_rel> ::= EQUAL | LESS_THAN | GREATER_THAN | LESS_THAN_EQUAL | GREATER_THAN_EQUAL | NOT_EQUAL
std::shared_ptr<ComplexBoolExpr> Parser::bool_rel()
{
	switch (cur_token.get_type())
	{
	case TokenType::EQUAL:
	case TokenType::LESS_THAN:
	case TokenType::GREATER_THAN:
	case TokenType::LESS_THAN_EQUAL:
	case TokenType::GREATER_THAN_EQUAL:
	case TokenType::NOT_EQUAL:
	{
		SP(ComplexBoolExpr, ret);
		ret->set_rel_type(cur_token.get_type());
		advance();
		return ret;
	}

	default: break;
	}

	return nullptr;
}

// Parser loop definition
// Grammar Rule:
// 	<loop> ::= WHILE <bexpr> DO <stmts> END
std::shared_ptr<WhileStmt> Parser::loop()
{
	// Create the WhileStmt object
	SP(WhileStmt, ret);

	// loop should currently only be called when a WHILE token is found
	eat(TokenType::WHILE, "expected while keyword");

	// Get the boolean expression object
	ret->set_while_expr(bexpr());

	// Finish the rule
	eat(TokenType::DO, "expected do keyword");
	SP(StmtList, stmtsp);
	stmts(stmtsp);
	eat(TokenType::END, "expected end keyword");

	// Add the statements to the WhileStmt object
	ret->set_while_stmts(stmtsp);

	return ret;
}
