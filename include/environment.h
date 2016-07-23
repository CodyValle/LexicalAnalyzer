#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED

// Declares the Environment class which keeps track of variable types and values.

#include <iostream>
#include <memory>
#include <unordered_map>

#include "token.h"

// Forward declaration for friend function operator<<
template<typename T>
class Environment;

// Print operator for a Environment object
template<typename T>
std::ostream& operator<<(std::ostream& out, const Environment<T>& value);

/// Stores references to variable data in the current scope
template<typename T>
class Environment
{
public:
  // Constructor
  Environment();
  // Destructor
  ~Environment();

	// Adds an identifier to the environment
	bool add_identifier(std::string, T&);

	// To check conditions whenever an identifier is found
	std::unique_ptr<T>* get_identifier(std::string);

private:
  // Stores the identifier data in this environment
  std::unordered_map<std::string, std::unique_ptr<T>> identifiers;

  // To access private members without accessors
  friend std::ostream& operator<< <>(std::ostream&, const Environment<T>&);
};

/// //////////////////////////////////
/// Environment definitions
/// //////////////////////////////////

// Environment Constructor
template<typename T>
Environment<T>::Environment() :
  identifiers(0)
{
}

// Environment Destructor
template<typename T>
Environment<T>::~Environment()
{
}

// Environment add_identifier definition
template<typename T>
bool Environment<T>::add_identifier(std::string key, T& data)
{
  // Check that the lexeme is not already here, otherwise redefinition error
  if (identifiers.count(key))
    return false;

  // Add to environment
  identifiers[key] = std::make_unique<T>(data);

  return true;
}

// Environment found_identifier definition
template<typename T>
std::unique_ptr<T>* Environment<T>::get_identifier(std::string key)
{
  // We need to check that it exists in the table
  if (identifiers.count(key))
    // The identifier is in the table
    return &identifiers.at(key);

  return nullptr;
}

// Print operator for a Environment object
template<typename T>
std::ostream& operator<<(std::ostream& out, const Environment<T>& value)
{
  // Print out every T object
  for (const auto& i: value.identifiers)
    out << "Identifier: [" << i.first << "]" << std::endl
        << (*i.second);

  return out;
}

#endif // ENVIRONMENT_H_INCLUDED
