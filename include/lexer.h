#ifndef LEXER_H
#define LEXER_H

// Declares the Lexer class

/* TODO:
 * -Lexer should be an interface for multiple types of lexers (comment lexer, different grammar lexers, etc.).
 * -The Lexer class should likely be a singleton.
 */

#include <istream>

#include "token.h"

// Reads the opened file and generates tokens
// Example:
//	Lexer lexer(input_file_stream);
//	while(t.get_type() != TokenType::EOS)
//		tokenArray.push_back(lexer.next_token());
class Lexer
{
public:
	// Constructor
	// @param input_stream: The opened file stream that needs to be tokenized
	Lexer(std::istream& input_stream);

	// Returns the next token from property input_stream
	Token next_token();

private:
	// The reference to the opened file stream
	std::istream& input_stream;

	// The current line reading from the file stream
	int line;

	// The current column form the file stream
	int column;

	// Internal function that gets the entire string when a quote is found.
	// Example:
	//	if (input_stream.get() == '"')
	//	{
	//		std::string str = getString();
	//		Token t = Token(TokenType::STRING, str, line, column);
	//	}
	std::string getString();

	// Internal function that gets the entire number when a digit is found.
	// Example:
	//	if (std::isdigit(input_stream.get()))
	//	{
	//		std::string str = getNumber();
	//		Token t = Token(TokenType::INT, str, line, column);
	//	}
	std::string getNumber();

	// Internal function that gets the entire word (alphanumeric and/or '_') when a character is found.
	// Example:
	//	if (std::isalpha(input_stream.get()))
	//	{
	//		std::string str = getWord();
	//		if (word.compare("if") == 0)
	//			t = Token(TokenType::IF, "if", line, column);
	//		else if (word.compare("then") == 0)
	//			t = Token(TokenType::THEN, "then", line, column);
	//	}
	std::string getWord();
};

#endif // LEXER_H
