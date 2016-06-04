// Contains definitions for everything in "exception.h"

#include <sstream>

#include "exception.h"

// Exception constructor definition
Exception::Exception(std::string msg, int line, int column, ExceptionType type) :
  errMsg("")
{
  // The string creator
  std::ostringstream ss;

  // What type of error is this?
  switch (type)
  {
  case ExceptionType::LEXER:
    ss << "Lexical";
    break;

  case ExceptionType::PARSER:
    ss << "Syntax";
    break;

  case ExceptionType::VARVISIT:
    ss << "Variable";
    break;
  }

  // The rest of the output data
  ss << " error at line " << line << ", column " << column << ": " + msg;

  // Save for output
  errMsg = ss.str();
}
