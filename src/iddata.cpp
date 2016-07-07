// Defines everything in environment.h

#include <iostream>

#include "iddata.h"

// IDData constructor
IDData::IDData(bool init, TokenType type, TokenType sub, unsigned l) :
  initialized(init),
  data_type(type),
  sub_type(sub),
  len(l)
{
}

// Print operator for a IDData object
std::ostream& operator<<(std::ostream& out, const IDData value)
{
  out << "  Initialized: [" << (value.is_initialized() ? "yes" : "no") << "]" << std::endl
      << "  Data Type:   [" << value.get_type();
  if (value.get_sub_type() != TokenType::UNKNOWN)
  {
    out << " " << value.get_sub_type();
    if (value.get_sub_type() == TokenType::ARRAY)
      out << " of length " << value.get_length();
  }
  out << "]" << std::endl;

  return out;
}
