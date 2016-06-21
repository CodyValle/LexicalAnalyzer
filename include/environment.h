#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED

// Declares the Environment class which keeps track of variable types and values.

#include <memory>
#include <unordered_map>

#include "token.h"

/// Stores all known data on the identifier it is linked to
class IDData
{
public:
  // Constructor
  IDData(bool, TokenType, TokenType = TokenType::UNKNOWN);

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

private:
  // Tells whether this variable has been initialized
  bool initialized;

  // The type of this identifier
  TokenType data_type;

  // The sub type of this identifier
  TokenType sub_type;
};

// Print operator for a IDData object
std::ostream& operator<<(std::ostream&, const IDData);

/// Stores variable data in the current scope
class Environment
{
public:
  // Constructor
  Environment();

	// Adds an identifier to the environment
	bool add_identifier(std::string, IDData&);

	// To check conditions whenever an identifier is found
	std::unique_ptr<IDData>* get_identifier(std::string);

private:
  // Stores the identifier data in this environment
  std::unordered_map<std::string, std::unique_ptr<IDData>> identifiers;

  // To access private members without accessors
  friend std::ostream& operator<<(std::ostream&, const Environment&);
};

// Print operator for a Environment object
std::ostream& operator<<(std::ostream&, const Environment&);

#endif // ENVIRONMENT_H_INCLUDED
