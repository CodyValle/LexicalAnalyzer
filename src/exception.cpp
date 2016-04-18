// Contains definitions for everything in "exception.h"

#include <sstream>

#include "exception.h"

// Exception constructor definition
Exception::Exception(std::string msg, int line, int column, ExceptionType type) :
    msg(msg),
    line(line),
    column(column),
    type(type)
{
}

// Exception what() definition
const char* Exception::what() const noexcept
{
    std::ostringstream ss;

    if(type == ExceptionType::LEXER)
        ss << "Lexical error at line ";
    else if(type == ExceptionType::PARSER)
        ss << "Syntax error at line ";

    ss << line << ", column " << column << ": " + msg;

    return ss.str().c_str();
}
