#ifndef CODELEXER_H_INCLUDED
#define CODELEXER_H_INCLUDED

#include "lexer.h"
#include "codeToken.h"

// Reads the opened file and generates tokens
// Example:
//	Lexer lexer(input_file_stream);
//	while(t.get_type() != TokenType::EOS)
//		tokenArray.push_back(lexer.next_token());
class CodeLexer : public Lexer
{
public:
	// Constructor
	// @param: The opened file stream that needs to be tokenized
	CodeLexer(std::istream&);

	// Returns the next token from input_stream
	Token next_token();

private:
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

#endif // CODELEXER_H_INCLUDED
