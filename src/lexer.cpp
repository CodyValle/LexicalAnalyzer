// The Lexer object
// Contains definitions for everything defined in "lexer.h"

#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cctype>

#include "token.h"
#include "lexer.h"
#include "exception.h"

// For ease of typing
#define TT TokenType

// Lexer constructor definition
Lexer::Lexer(std::istream& stream) :
	input_stream(stream),
	line(1),
	column(0)
{
}

// Lext next_token() definition
Token Lexer::next_token()
{
	// We are taking one byte at a time
	char c = input_stream.peek();

	// If it's a newline character, reset some values and get the next token
	if (c == '\n')
	{
		line++;
		column = 0;
		input_stream.get();
		return next_token();
	}

	// Otherwise increment column
	column++;

	// Check for specific single-character token possibilities
	switch (c)
	{
	case ' ': // Whitespace. Skip
		input_stream.get();
		return next_token();

	case '\t': // Tab character. Skip
		input_stream.get();
		return next_token();

	case ';': // Semicolon
		input_stream.get();
		return Token(TT::SEMICOLON, ";", line, column);

	case ',': // Comma
		input_stream.get();
		return Token(TT::COMMA, ",", line, column);

	case '+': // Addition
		input_stream.get();
		return Token(TT::PLUS, "+", line, column);

	case '-': // Subtraction
		input_stream.get();
		return Token(TT::MINUS, "-", line, column);

	case '*': // Multiplication
		input_stream.get();
		return Token(TT::MULTIPLY, "*", line, column);

	case '/': // Division
		input_stream.get();
		return Token(TT::DIVIDE, "/", line, column);

	case '[': // Left bracket
		input_stream.get();
		return Token(TT::LBRACKET, "[", line, column);

	case ']': // Right bracket
		input_stream.get();
		return Token(TT::RBRACKET, "]", line, column);

	case '(': // Left parenthese
		input_stream.get();
		return Token(TT::LPAREN, "(", line, column);

	case ')': // Right parenthese
		input_stream.get();
		return Token(TT::RPAREN, ")", line, column);

	case EOF: // End of file
		return Token(TT::EOS, "EOF", line, column);

	case '#': // Comment
		do { input_stream.get(); }
		while (input_stream.peek() != '\n');
		return next_token();

	case '"': // String
	{
		input_stream.get();
		std::string str = get_string();
		Token t = Token(TT::STRING, str, line, column);
		column += str.length() + 1;
		return t;
	}

	case '!': // Either a NOT_EQUAL token or unknown token
	{
		input_stream.get();
		if (input_stream.peek() != '=')
			return Token(TT::UNKNOWN, "!",line, column);
		input_stream.get();
		column++;
		return Token(TT::NOT_EQUAL, "!=", line, column);
	}

	case '=': // Either ASSIGN token or EQUAL token
	{
		input_stream.get();
		c = input_stream.peek();
		if (c == '=')
		{
			input_stream.get();
			column++;
			return Token(TT::EQUAL, "==", line, column);
		}
		return Token(TT::ASSIGN, "=", line, column);
	}

	case '<': // Either LESS_THAN or LESS_THAN_EQUAL
	{
		input_stream.get();
		c = input_stream.peek();
		if (c == '=')
		{
			input_stream.get();
			column++;
			return Token(TT::LESS_THAN_EQUAL, "<=", line, column);
		}
		return Token(TT::LESS_THAN, "<", line, column);
	}

	case '>': // Either GREATER_THAN or GREATER_THAN_EQUAL
	{
		input_stream.get();
		c = input_stream.peek();
		if (c == '=')
		{
			input_stream.get();
			column++;
			return Token(TT::GREATER_THAN_EQUAL, "<=", line, column);
		}
		return Token(TT::GREATER_THAN, "<", line, column);
	}
	} // switch (c)

	// Is this a start to a variable or keyword?
	if (std::isalpha(c))
	{
		// Get the entire word
		std::string word = get_word();
		TT t;

		if (word.compare("while") == 0)
			t = TT::WHILE;
		else if (word.compare("do") == 0)
			t = TT::DO;
		else if (word.compare("end") == 0)
			t = TT::END;
		else if (word.compare("readint") == 0)
			t = TT::READINT;
		else if (word.compare("readstr") == 0)
			t = TT::READSTR;
		else if (word.compare("print") == 0)
			t = TT::PRINT;
		else if (word.compare("println") == 0)
			t = TT::PRINTLN;
		else if (word.compare("and") == 0)
			t = TT::AND;
		else if (word.compare("or") == 0)
			t = TT::OR;
		else if (word.compare("not") == 0)
			t = TT::NOT;
		else if (word.compare("if") == 0)
			t = TT::IF;
		else if (word.compare("then") == 0)
			t = TT::THEN;
		else if (word.compare("elif") == 0)
			t = TT::ELIF;
		else if (word.compare("else") == 0)
			t = TT::ELSE;
		else if (word.compare("true") == 0)
			t = TT::BOOL;
		else if (word.compare("false") == 0)
			t = TT::BOOL;
		else if (word.compare("string") == 0)
			t = TT::STRINGT;
		else if (word.compare("int") == 0)
			t = TT::INTEGER;
		else if (word.compare("float") == 0)
			t = TT::FLOAT;
		else if (word.compare("char") == 0)
			t = TT::CHAR;
		else if (word.compare("boolean") == 0)
			t = TT::BOOLEAN;
		else // It's not a keyword, so it must be a variable
			t = TT::ID;

		Token token = Token(t, word, line, column);
		column += word.length() - 1;
		return token;
	}

	// Is this a number?
	if (std::isdigit(c))
	{
		// Read the entire number
		std::string number = get_number();
		column += number.length() - 1;
		return Token(TT::INT, number, line, column);
	}

	// Only reached when a case is not accounted for.
	input_stream.get();
	return Token(TT::UNKNOWN, std::string(1,c), line, column);
}

// Lexer getNumber() definition
std::string Lexer::get_number()
{
	std::ostringstream ss;
	char c = input_stream.get();

	while (true)
	{
		ss << c;
		c = input_stream.peek();
		if (std::isdigit(c))
			input_stream.get();
		else break;
	}

  return ss.str();
}

// Lexer getString() definition
std::string Lexer::get_string()
{
	std::ostringstream ss;
	char c = input_stream.get();

	while (true)
	{
		ss << c;
		c = input_stream.get();
		if (c == '"') break;

		// newline characters are not allowed in strings
		if (c == '\n')
			throw Exception("Found newline in string. Strings must start and end on the same line.", line, column, ExceptionType::LEXER);
	}

  return ss.str();
}

// Lexer getWord() definition
std::string Lexer::get_word()
{
	std::ostringstream ss;
	char c = input_stream.get();

	while (true)
	{
		ss << c;
		c = input_stream.peek();
		if (std::isalnum(c) || c == '_')
			input_stream.get();
		else break;
	}

  return ss.str();
}
