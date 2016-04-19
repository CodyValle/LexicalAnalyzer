#ifndef PARSER_H
#define PARSER_H

// Parses the token stream given by the Lexer

#include <memory>

#include "lexer.h"
#include "token.h"

// Parses the token stream given by the lexer
// Example:
// 	Lexer lexer(input_file_stream);
// 	Parser parser(lexer);
// 	Parser.parse();
class Parser
{
public:
	// Constructor
	// Takes a lexer object as the source for the token stream
	Parser(Lexer& lexer);

	// Does the meat of the parser duties
	void parse();

private:
	// Reference to the lexer object
	Lexer& lexer;

	// Reference to the current token
	Token current_token;

	/// Helper functions
	// Advances the parser to the next token
	inline void advance()
	{ current_token = lexer.next_token(); }

	// Takes an expected token from the stream.
	// Throws an error using @param msg if the token is not what was expected
	void eat(TokenType token_type, const std::string& msg);

	// Reports the error encountered
	void error(const std::string& msg);

	// Applies the stmts rule
	// Grammar Rule:
	// 	<stmts> ::= <stmt> <stmts> | empty
	void stmts();

	// Applies the stmt rule
	// Grammar Rule":
	// 	<stmt>::= <output> | <assign> | <cond> | <loop>
	void stmt();

	// Applies the output rule
	// Grammar Rule:
	// 	<output> ::= PRINT LPAREN <expr> RPAREN SEMICOLON | PRINTLN LPAREN <expr> RPAREN SEMICOLON
	void output();


	// Applies the input rule
	// Grammar Rule:
	// 	<input> ::= READINT LPAREN <string> RPAREN | READSTR LPAREN <string> RPAREN
	void input();

	// Applies the asing rule
	// Grammar Rule:
	// 	<output> ::= ID <listindex> ASSIGN <expr> SEMICOLON
	void assign();

	// Applies the listindex rule
	// Grammar Rule:
	// 	<listindex> ::= LBRACKET <expr> RBRACKET | empty
	void listindex();

	// Applies the expr rule
	// Grammar Rule:
	// 	<expr> ::= <value> <exprt>
	void expr();

	// Applies the exprt rule
	// Grammar Rule:
	// 	<exprt> ::= <math_rel> <expr> | <bool_rel> <expr> | empty
	void exprt();

	// Applies the value rule
	// Grammar Rule:
	// 	<value> ::= ID <listindex> | STRING | INT | BOOL | <input> | LBRACKET <exprlist> RBRACKET
	void value();

	// Applies the cond rule
	// Grammar Rule:
	// <exprlist> ::= <expr> <exprtail> | empty
	void exprlist();

	// Applies the cond rule
	// Grammar Rule:
	// 	<exprtail> ::= COMMA <expr> <exprtail> | empty
	void exprtail();

	// Applies the cond rule
	// Grammar Rule:
	// 	<math_rel> ::= PLUS | MINUS | DIVIDE | MULTIPLY
	bool math_rel();

	// Applies the cond rule
	// Grammar Rule:
	// 	<cond> ::= IF <bexpr> THEN <stmts> <condt> END
	void cond();

	// Applies the condt rule
	// Grammar Rule:
	// 	<condt> ::= ELIF <bexpr> THEN <stmts> <condt> | ELSE <stmts> | empty
	void condt();

	// Applies the bexpr rule
	// Grammar Rule:
	// 	<bexpr> ::= <expr> <bexprt> | NOT <expr> <bexprt>
	void bexpr();

	// Applies the bexpr rule
	// Grammar Rule:
	// 	<bexprt> ::= <bool_rel> <expr> <bconnct> | empty
	void bexprt();

	// Applies the bexpr rule
	// Grammar Rule:
	// 	<bconnct> ::= AND <bexpr> | OR <bexpr> | empty
	void bconnct();

	// Applies the bexpr rule
	// Grammar Rule:
	// 	<bool_rel> ::= EQUAL | LESS_THAN | GREATER_THAN | LESS_THAN_EQUAL | GREATER_THAN_EQUAL | NOT_EQUAL
	bool bool_rel();

	// Applies the loop rule
	// Grammar Rule:
	// 	<loop> ::= WHILE <bexpr> DO <stmts> END
	void loop();
};

#endif // PARSER_H
