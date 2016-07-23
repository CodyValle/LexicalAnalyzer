// Defines everything in vardata.h

#include <iostream>

#include "vardata.h"

// VarData constructor
VarData::VarData(TokenType type, TokenType subtype, unsigned length) :
  data_type(type),
  sub_type(subtype),
  len(length),
  int_val(0)
{
  int_val = new int[len];
}

VarData::VarData(const VarData& other) :
  data_type(other.get_type()),
  sub_type(other.get_sub_type()),
  len(other.get_length()),
  int_val(0)
{
  int_val = new int[len];

  for (unsigned i = 0; i < len; ++i)
    int_val[i] = other.get_int_value(i);
}

// VarData destructor
VarData::~VarData()
{
  delete int_val;
}
