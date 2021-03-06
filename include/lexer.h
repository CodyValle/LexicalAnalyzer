#ifndef LEXER_H
#define LEXER_H

// Declares the Lexer class

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
	// @param: The opened file stream that needs to be tokenized
	Lexer(std::istream&);

	// Returns the next token from input_stream
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
	std::string get_string();

	// Internal function that gets the entire number when a digit is found.
	// Example:
	//	if (std::isdigit(input_stream.get()))
	//	{
	//		std::string str = getNumber();
	//		Token t = Token(TokenType::INT, str, line, column);
	//	}
	std::string get_number();

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
	std::string get_word();
};

#endif // LEXER_H
