// Defines everything in vardata.h

#include <iostream>

#include "vardata.h"

// VarData constructor
VarData::VarData(TokenType type, TokenType subtype, unsigned length) :
  data_type(type),
  sub_type(subtype),
  len(length),
  value(0)
{
  value = new all_type[len];
}

VarData::VarData(const VarData& other) :
  data_type(other.get_type()),
  sub_type(other.get_sub_type()),
  len(other.get_length()),
  value(0)
{
  value = new all_type[len];

  for (unsigned i = 0; i < len; ++i)
    value[i] = other.get_value(i);
}

// VarData destructor
VarData::~VarData()
{
  delete value;
}
