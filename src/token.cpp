// Contains definitions for the TokenType print operator
// and definitions for Token constructors and Token print operator.

#include <iostream>
#include <string>

#include "token.h"

#define TT TokenType

// Print operator for a TokenType object
std::ostream& operator<<(std::ostream& out, const TokenType value)
{
	switch (value)
	{
	case TT::PRINT:     return out << "PRINT";
	case TT::PRINTLN:   return out << "PRINTLN";
	case TT::READINT:   return out << "READINT";
	case TT::READSTR:   return out << "READSTR";
	case TT::LPAREN:    return out << "LPAREN";
	case TT::RPAREN:    return out << "RPAREN";
	case TT::SEMICOLON: return out << "SEMICOLON";
	case TT::ID:        return out << "ID";
	case TT::LBRACKET:  return out << "LBRACKET";
	case TT::RBRACKET:  return out << "RBRACKET";
	case TT::STRING:    return out << "STRING";
	case TT::STRINGT:   return out << "STRINGT";
	case TT::INT:       return out << "INT";
	case TT::INTEGER:   return out << "INTEGER";
	case TT::FLOAT:     return out << "FLOAT";
	case TT::CHAR:      return out << "CHAR";
	case TT::BOOLEAN:   return out << "BOOLEAN";
	case TT::ARRAY  :   return out << "ARRAY";
	case TT::COMMA:     return out << "COMMA";
	case TT::ASSIGN:    return out << "ASSIGN";
	case TT::PLUS:      return out << "PLUS";
	case TT::MINUS:     return out << "MINUS";
	case TT::DIVIDE:    return out << "DIVIDE";
	case TT::MULTIPLY:  return out << "MULTIPLY";
	case TT::IF:        return out << "IF";
	case TT::THEN:      return out << "THEN";
	case TT::ELIF:      return out << "ELIF";
	case TT::ELSE:      return out << "ELSE";
	case TT::END:       return out << "END";
	case TT::NOT:       return out << "NOT";
	case TT::AND:       return out << "AND";
	case TT::OR:        return out << "OR";
	case TT::EQUAL:     return out << "EQUAL";
	case TT::LESS_THAN: return out << "LESS_THAN";
	case TT::GREATER_THAN: return out << "GREATER_THAN";
	case TT::LESS_THAN_EQUAL: return out << "LESS_THAN_EQUAL";
	case TT::GREATER_THAN_EQUAL: return out << "GREATER_THAN_EQUAL";
	case TT::NOT_EQUAL: return out << "NOT_EQUAL";
	case TT::WHILE:     return out << "WHILE";
	case TT::DO:        return out << "DO";
	case TT::EOS:       return out << "EOS";
	case TT::DEFAULT:   return out << "DEFAULT";
	case TT::UNKNOWN:   return out << "UNKNOWN";
	case TT::BOOL:      return out << "BOOL";
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
