#ifndef ALL_TYPE_H_INCLUDED
#define ALL_TYPE_H_INCLUDED

#include "boost/variant.hpp"

// Defines a variable of any type
typedef boost::variant<int, bool, std::string> all_type;

/// Definitions of classes that convert an all_type object to a different type
// Converts to an int
class int_visitor : public boost::static_visitor<int>
{
public:
  int operator()(const int val) const
  { return val; }

  int operator()(const bool val) const
  { return val; }

  int operator()(const std::string val) const
  { return std::stoi(val); }
};

// Converts to a bool
class bool_visitor : public boost::static_visitor<bool>
{
public:
  bool operator()(const int val) const
  { return val == 0; }

  bool operator()(const bool val) const
  { return val; }

  bool operator()(const std::string& val) const
  { return val.compare("true") == 0; }
};

// Converts to a string
class string_visitor : public boost::static_visitor<std::string>
{
public:
  std::string operator()(const int val) const
  { return std::to_string(val); }

  std::string operator()(const bool val) const
  { return val ? "true" : "false"; }

  std::string operator()(const std::string val) const
  { return val; }
};

#endif // ALL_TYPE_H_INCLUDED
