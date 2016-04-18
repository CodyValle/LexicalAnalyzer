// Contains definitions for the TokenType print operator
// and definitions for Token constructors and Token print operator.

#include <iostream>
#include <string>

#include "token.h"

// Print operator for a TokenType object
std::ostream& operator<<(std::ostream& out, const TokenType value)
{
	switch (value)
	{
	case TokenType::PRINT:     return out << "PRINT";
	case TokenType::PRINTLN:   return out << "PRINTLN";
	case TokenType::READINT:   return out << "READINT";
	case TokenType::READSTR:   return out << "READSTR";
	case TokenType::LPAREN:    return out << "LPAREN";
	case TokenType::RPAREN:    return out << "RPAREN";
	case TokenType::SEMICOLON: return out << "SEMICOLON";
	case TokenType::ID:        return out << "ID";
	case TokenType::LBRACKET:  return out << "LBRACKET";
	case TokenType::RBRACKET:  return out << "RBRACKET";
	case TokenType::STRING:    return out << "STRING";
	case TokenType::INT:       return out << "INT";
	case TokenType::BOOL:      return out << "BOOL";
	case TokenType::COMMA:     return out << "COMMA";
	case TokenType::ASSIGN:    return out << "ASSIGN";
	case TokenType::PLUS:      return out << "PLUS";
	case TokenType::MINUS:     return out << "MINUS";
	case TokenType::DIVIDE:    return out << "DIVIDE";
	case TokenType::MULTIPLY:  return out << "MULTIPLY";
	case TokenType::IF:        return out << "IF";
	case TokenType::THEN:      return out << "THEN";
	case TokenType::ELIF:      return out << "ELIF";
	case TokenType::ELSE:      return out << "ELSE";
	case TokenType::END:       return out << "END";
	case TokenType::NOT:       return out << "NOT";
	case TokenType::AND:       return out << "AND";
	case TokenType::OR:        return out << "OR";
	case TokenType::EQUAL:     return out << "EQUAL";
	case TokenType::LESS_THAN: return out << "LESS_THAN";
	case TokenType::GREATER_THAN: return out << "GREATER_THAN";
	case TokenType::LESS_THAN_EQUAL: return out << "LESS_THAN_EQUAL";
	case TokenType::GREATER_THAN_EQUAL: return out << "GREATER_THAN_EQUAL";
	case TokenType::NOT_EQUAL: return out << "NOT_EQUAL";
	case TokenType::WHILE:     return out << "WHILE";
	case TokenType::DO:        return out << "DO";
	case TokenType::EOS:       return out << "EOS";
	case TokenType::COMMENT:   return out << "COMMENT";
	case TokenType::DEFAULT:   return out << "DEFAULT_TOKEN";
	case TokenType::UNKNOWN:   return out << "UNKNOWN_TOKEN";
	case TokenType::TRUE:      return out << "TRUE";
	case TokenType::FALSE:     return out << "FALSE";
	}

	return out << "UNKNOWN_TOKEN_TYPE";
}

// Token constructor definition for a default token
Token::Token() :
	type(TokenType::DEFAULT),
	lexeme(""),
	line(-1),
	column(-1)
{
}

// Token constructor definition for a specified token
Token::Token(TokenType type, std::string lexeme, int line, int column) :
	type(type),
	lexeme(lexeme),
	line(line),
	column(column)
{
}

// Print operator for a Token object
std::ostream& operator<<(std::ostream& out, const Token token)
{
	// Example output:
	//	PRINT{"print"}(3,1)
	return out << token.get_type() << "{" << token.get_lexeme() << "}(" << token.get_line() << "," << token.get_column() << ")";
}
