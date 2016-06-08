// Defines everything in environment.h

#include <iostream>

#include "environment.h"

// IDData constructor
IDData::IDData(bool init, TokenType type, TokenType sub) :
  initialized(init),
  data_type(type),
  sub_type(sub)
{
}

// Print operator for a IDData object
std::ostream& operator<<(std::ostream& out, const IDData value)
{
  out << "  Initialized: [" << (value.is_initialized() ? "false" : "true") << "]" << std::endl
      << "  Data Type:   [" << value.get_type();
  if (value.get_sub_type() != TokenType::UNKNOWN)
    out << " " << value.get_sub_type();
  out << "]" << std::endl;

  return out;
}

// Environment Constructor
Environment::Environment() :
  identifiers(0)
{
}

// Print operator for a Environment object
std::ostream& operator<<(std::ostream& out, const Environment& value)
{
  // Print out every IDData object
  for (const auto& i: value.identifiers)
    out << "Identifier: [" << i.first << "]" << std::endl
        << (*i.second);

  return out;
}

// Environment add_identifier definition
bool Environment::add_identifier(std::string key, IDData& data)
{
  // Check that the lexeme is not already here, otherwise redefinition error
  if (identifiers.count(key))
    return false;

  // Add to environment
  identifiers[key] = std::make_unique<IDData>(data);

  return true;
}

// Environment found_identifier definition
std::unique_ptr<IDData>* Environment::get_identifier(std::string key)
{
  // We need to check that it exists in the table
  if (identifiers.count(key))
    // The identifier is in the table
    return &identifiers.at(key);

  return nullptr;
}
