#ifndef TOKEN_H
#define TOKEN_H

// Declares the Token class
// and the TokenType enum class

#include <string>

// The type of token that the Token class represents.
enum class TokenType
{
  PRINT, PRINTLN, READINT, READSTR, LPAREN, RPAREN, SEMICOLON,
  ID, LBRACKET, RBRACKET, STRING, INT, FLOAT, CHAR,
  ARRAY, VAR, COMMA, ASSIGN, PLUS, MINUS, DIVIDE, MULTIPLY, IF, THEN,
  ELIF, ELSE, END, NOT, AND, OR, EQUAL,LESS_THAN, GREATER_THAN,
  LESS_THAN_EQUAL, GREATER_THAN_EQUAL, NOT_EQUAL, WHILE, DO,
  EOS, DEFAULT, BOOL, UNKNOWN
};

// Print operator for a TokenType object
std::ostream& operator<<(std::ostream&, const TokenType);

// A token that was found by the lexer.
// Create example:
//  Token token(CodeTokenType::IF, "if", 12, 0);
// Use example:
//  switch (token.get_type())
//  {
//      case CodeTokenType::INT:
//          break;
//  ...
//  }
class Token
{
public:
    // Constructor
    // Creates a default token. Likely, the parser will not know what to do with this.
    Token();

    // Constructor
    // Creates a token of specified type.
    // @param type: The CodeTokenType that this token represents.
    // @param lexeme: The raw value of this token.
    // @param line: The line where this token can be found.
    // @param column: The column where this token starts.
    Token(TokenType, std::string lexeme, int line, int column);

    // Returns the TokenType of this token
    inline TokenType get_type() const
        { return type; }

    // Returns the raw value of this token
    inline std::string get_lexeme() const
        { return lexeme; }

    // Returns the line where this token resides
    inline int get_line() const
        { return line; }

    // Returns the column where this token starts
    inline int get_column() const
        { return column; }

private:
    // The type of token this is
    TokenType type;

    // The raw value of this token
    std::string lexeme;

    // The line this token exists on
    int line;

    // The column this token starts on
    int column;
};

// Print operator for a Token object
std::ostream& operator<<(std::ostream&, const Token);

#endif // TOKEN_H
