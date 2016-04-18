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

// Lexer constructor definition
Lexer::Lexer(std::istream& input_stream) :
	input_stream(input_stream),
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
		return Token(TokenType::SEMICOLON, ";", line, column);

	case ',': // Comma
		input_stream.get();
		return Token(TokenType::COMMA, ",", line, column);

	case '+': // Addition
		input_stream.get();
		return Token(TokenType::PLUS, "+", line, column);

	case '-': // Subtraction
		input_stream.get();
		return Token(TokenType::MINUS, "-", line, column);

	case '*': // Multiplication
		input_stream.get();
		return Token(TokenType::MULTIPLY, "*", line, column);

	case '/': // Division
		input_stream.get();
		return Token(TokenType::DIVIDE, "/", line, column);

	case '{': // Left bracket
		input_stream.get();
		return Token(TokenType::LBRACKET, "{", line, column);

	case '}': // Right bracket
		input_stream.get();
		return Token(TokenType::RBRACKET, "}", line, column);

	case '(': // Left parenthese
		input_stream.get();
		return Token(TokenType::LPAREN, "(", line, column);

	case ')': // Right parenthese
		input_stream.get();
		return Token(TokenType::RPAREN, ")", line, column);

	case EOF: // End of file
		return Token(TokenType::EOS, "EOF", line, column);

	case '#': // Comment
		do { input_stream.get(); }
		while (input_stream.peek() != '\n');
		return Token(TokenType::COMMENT, "#...",line,column);

	case '"': // String
	{
		input_stream.get();
		std::string str = getString();
		Token t = Token(TokenType::STRING, str, line, column);
		column += str.length() + 1;
		return t;
	}

	case '!': // Either a NOT_EQUAL token or unknown token
	{
		input_stream.get();
		if (input_stream.peek() != '=')
			return Token(TokenType::UNKNOWN, "!",line, column);
		input_stream.get();
		column++;
		return Token(TokenType::NOT_EQUAL, "!=", line, column);
	}

	case '=': // Either ASSIGN token or EQUAL token
	{
		input_stream.get();
		c = input_stream.peek();
		if (c == '=')
		{
			input_stream.get();
			column++;
			return Token(TokenType::EQUAL, "==", line, column);
		}
		return Token(TokenType::ASSIGN, "=", line, column);
	}

	case '<': // Either LESS_THAN or LESS_THAN_EQUAL
	{
		input_stream.get();
		c = input_stream.peek();
		if (c == '=')
		{
			input_stream.get();
			column++;
			return Token(TokenType::LESS_THAN_EQUAL, "<=", line, column);
		}
		return Token(TokenType::LESS_THAN, "<", line, column);
	}

	case '>': // Either GREATER_THAN or GREATER_THAN_EQUAL
	{
		input_stream.get();
		c = input_stream.peek();
		if (c == '=')
		{
			input_stream.get();
			column++;
			return Token(TokenType::GREATER_THAN_EQUAL, "<=", line, column);
		}
		return Token(TokenType::GREATER_THAN, "<", line, column);
	}
	} // switch (c)

	// Is this a start to a variable or keyword?
	if (std::isalpha(c))
	{
		// Get the entire word
		std::string word = getWord();
		TokenType t;

		if (word.compare("while") == 0)
			t = TokenType::WHILE;
		else if (word.compare("do") == 0)
			t = TokenType::DO;
		else if (word.compare("end") == 0)
			t = TokenType::END;
		else if (word.compare("readint") == 0)
			t = TokenType::READINT;
		else if (word.compare("readstr") == 0)
			t = TokenType::READSTR;
		else if (word.compare("print") == 0)
			t = TokenType::PRINT;
		else if (word.compare("println") == 0)
			t = TokenType::PRINTLN;
		else if (word.compare("and") == 0)
			t = TokenType::AND;
		else if (word.compare("or") == 0)
			t = TokenType::OR;
		else if (word.compare("not") == 0)
			t = TokenType::NOT;
		else if (word.compare("if") == 0)
			t = TokenType::IF;
		else if (word.compare("then") == 0)
			t = TokenType::THEN;
		else if (word.compare("elif") == 0)
			t = TokenType::ELIF;
		else if (word.compare("else") == 0)
			t = TokenType::ELSE;
		else if (word.compare("true") == 0)
			t = TokenType::TRUE;
		else if (word.compare("false") == 0)
			t = TokenType::FALSE;
		else // It's not a keyword, so it must be a variable
			t = TokenType::ID;

		Token token = Token(t, word, line, column);
		column += word.length() - 1;
		return token;
	}

	// IS this a number?
	if (std::isdigit(c))
	{
		// Read the entire number
		std::string number = getNumber();
		column += number.length() - 1;
		return Token(TokenType::INT, number, line, column);
	}

	// Only reached when a case is not accounted for.
	input_stream.get();
	return Token(TokenType::UNKNOWN, std::string(1,c), line, column);
}

// Lexer getNumber() definition
std::string Lexer::getNumber()
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
std::string Lexer::getString()
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
std::string Lexer::getWord()
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
