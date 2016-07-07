#ifndef IDDATA_H_INCLUDED
#define IDDATA_H_INCLUDED

#include <iostream>

#include "token.h"

/// Stores all known data on the identifier it is linked to
class IDData
{
public:
  // Constructor
  IDData(bool, TokenType, TokenType = TokenType::UNKNOWN, unsigned = 0);

  // Returns the value of initialized
  bool is_initialized() const
    { return initialized; }

  // Sets the variable as initialized
  void initialize()
    { initialized = true; }

  // Get the type of this identifier
  TokenType get_type() const
    { return data_type; }

  // Get the sub type of this identifier
  TokenType get_sub_type() const
    { return sub_type; }

  // Get the length of the list
  unsigned get_length() const
    { return len; }

private:
  // Tells whether this variable has been initialized
  bool initialized;

  // The type of this identifier
  TokenType data_type;

  // The sub type of this identifier
  TokenType sub_type;

  // The length of the list, if the sub type is TokenType::ARRAY
  unsigned len;
};

// Print operator for a IDData object
std::ostream& operator<<(std::ostream&, const IDData);

#endif // IDDATA_H_INCLUDED
