#ifndef VARDATA_H_INCLUDED
#define VARDATA_H_INCLUDED

#include <iostream>

#include "token.h"

/// Stores all known data on the variable it is linked to
class VarData
{
public:
  // Constructor
  // @param type: The type of the variable
  // @param subtype: The subtype of the variable
  // @param length: The length of the list, if the variable is a list
  VarData(TokenType type, TokenType subtype = TokenType::UNKNOWN, unsigned length = 1);

  // Copy constructor
  VarData(const VarData&);

  // Destructor
  ~VarData();

  // Get the type of this identifier
  TokenType get_type() const
    { return data_type; }

  // Get the sub type of this identifier
  TokenType get_sub_type() const
    { return sub_type; }

  // Get the length of the list
  unsigned get_length() const
    { return len; }

  // Get the length of the list
  void set_length(unsigned l)
    {
      delete int_val;
      int_val = new int[l];
    }

  // Get the integer value of this variable
  void set_int_value(int val, unsigned index = 0)
    {
      if (index >= len) {std::cerr << "Out of bounds access." << std::endl; return;}
      int_val[index] = val;
    }

  // Get the integer value of this variable
  int get_int_value(unsigned index = 0) const
    {
      if (index >= len) {std::cerr << "Out of bounds access." << std::endl; return 0;}
      return int_val[index];
    }

  // Assignment overload
  VarData& operator=(const VarData& other)
    {
      if (this == &other) return *this;

      data_type = other.get_type();
      sub_type = other.get_sub_type();
      len = other.get_length();

      int_val = new int[len];

      for (unsigned i = 0; i < len; ++i)
        int_val[i] = other.get_int_value(i);

      return *this;
    }

private:
  // The type of this identifier
  TokenType data_type;

  // The sub type of this identifier
  TokenType sub_type;

  // The length of the list, if the sub type is TokenType::ARRAY
  unsigned len;

  // The int value(s) of this variable
  int* int_val;
};

#endif // VARDATA_H_INCLUDED
