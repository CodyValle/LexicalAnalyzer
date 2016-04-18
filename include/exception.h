#ifndef EXCEPTION_H
#define EXCEPTION_H

// Declares the Exception class
// and the ExceptionType enum class

#include <exception>
#include <string>

// Specifies the type of Exception this is.
// LEXER: The lexer threw an exception.
// PARSER: The parser threw an exception.
enum class ExceptionType {
LEXER, PARSER
};

// Extends the standard exception class for this lexer and parser.
// Specifies the error that was encountered.
// Example:
//	try { ... }
//	catch(Exception e)
//	{
//		std::cout << e.what() << std::endl;
//	}
class Exception : public std::exception
{
public:
	// Constructor
	// @param msg: The message to print when what() is called.
	// @param line: The line where the error occurred.
	// @param column: The column where the error occurred.
	Exception(std::string msg, int line, int column, ExceptionType type);

	// An explanation of what and where the error is.
	virtual const char* what() const noexcept;

private:
	// The message that was passed in when this Exception was thrown.
	std::string msg;

	// The line that this Exception is referencing
	int line;

	// The column that this Exception is referencing
	int column;

	// The type of Exception this is
	ExceptionType type;
};

#endif // EXCEPTION_H
