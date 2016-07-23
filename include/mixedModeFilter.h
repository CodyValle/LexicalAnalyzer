#ifndef MIXEDMODEFILTER_H_INCLUDED
#define MIXEDMODEFILTER_H_INCLUDED

#include "all_type.h"

// Possible return types
enum Type {INT = 0, BOOL, STRING};

// Determines the current type of the all_type variable
class all_type_visitor : public boost::static_visitor<Type>
{
public:
  Type operator()(const int) const
    { return INT; }

  Type operator()(const bool) const
    { return BOOL; }

  Type operator()(const std::string) const
    { return STRING; }
};

// Specialized template to perform boolean operators on various types
// Helps ComplexBoolExpr
template<typename T1, typename T2>
class DoBoolOperation
{
private:
  // The output of the operation
  all_type ret;

  /// Common operators
  void equ(T1 a, T2 b)
    { ret = all_type(a == b); }

  void lt(T1 a, T2 b)
    { ret = all_type(a < b); }

  void gt(T1 a, T2 b)
    { ret = all_type(a > b); }

  void lte(T1 a, T2 b)
    { ret = all_type(a <= b); }

  void gte(T1 a, T2 b)
    { ret = all_type(a >= b); }

  void neq(T1 a, T2 b)
    { ret = all_type(a != b); }

public:
  DoBoolOperation(T1 a, T2 b, TokenType op) :
    ret(all_type(0))
  {
    switch (op)
    {
      case TokenType::EQUAL:
        equ(a, b);
        break;

      case TokenType::LESS_THAN:
        lt(a, b);
        break;

      case TokenType::GREATER_THAN:
        gt(a, b);
        break;

      case TokenType::LESS_THAN_EQUAL:
        lte(a, b);
        break;

      case TokenType::GREATER_THAN_EQUAL:
        gte(a, b);
        break;

      case TokenType::NOT_EQUAL:
        neq(a, b);
        break;

      default: break;
    }
  }

  // Gets the output of the operation
  all_type get()
    { return ret; }
};

// Specialized template to perform mathematical operators on various types
// Helps ComplexExpr
template<typename T1, typename T2>
class DoMathOperation
{
private:
  // The output of the operation
  all_type ret;

  /// Common operators
  void add(T1 a, T2 b)
    { ret = all_type(a + b); }

  void sub(T1 a, T2 b)
    { ret = all_type(a - b); }

  void mul(T1 a, T2 b)
    { ret = all_type(a * b); }

  void div(T1 a, T2 b)
    { ret = all_type(a / b); }

public:
  DoMathOperation(T1 a, T2 b, TokenType op) :
    ret(all_type(0))
  {
    switch (op)
    {
      case TokenType::PLUS:
        add(a, b);
        break;

      case TokenType::MINUS:
        sub(a, b);
        break;

      case TokenType::MULTIPLY:
        mul(a, b);
        break;

      case TokenType::DIVIDE:
        div(a, b);
        break;

      default: break;
    }
  }

  // Gets the output of the operation
  all_type get()
    { return ret; }
};

/// Specialized functions follow. Most are here.

/// int and bool
template<>
void DoMathOperation<int, bool>::mul(int a, bool)
  { std::cerr << "cannot multiply the integer '" << a << "' by a boolean, " << std::endl; }

template<>
void DoMathOperation<int, bool>::div(int a, bool)
  { std::cerr << "cannot divide the integer '" << a << "' by a boolean, " << std::endl; }

/// int and string
template<>
void DoMathOperation<int, std::string>::add(int a, std::string b)
  { std::cerr << "cannot add the string '" << b << "' to the integer '" << a << "', " << std::endl; }

template<>
void DoMathOperation<int, std::string>::sub(int a, std::string b)
  { std::cerr << "cannot subtract the string '" << b << "' from the integer '" << a << "', " << std::endl; }

template<>
void DoMathOperation<int, std::string>::mul(int a, std::string b)
  { std::cerr << "cannot multiply the integer '" << a << "' by the string '" << b << "', " << std::endl; }

template<>
void DoMathOperation<int, std::string>::div(int a, std::string b)
  { std::cerr << "cannot divide the integer '" << a << "' by the string '" << b << "', " << std::endl; }

/// bool and int
template<>
void DoMathOperation<bool, int>::add(bool, int b)
  { std::cerr << "cannot add the integer '" << b << "' to a boolean, " << std::endl; }

template<>
void DoMathOperation<bool, int>::sub(bool, int b)
  { std::cerr << "cannot subtract the integer '" << b << "' to a boolean, " << std::endl; }

template<>
void DoMathOperation<bool, int>::mul(bool, int b)
  { std::cerr << "cannot multiply a boolean by the integer '" << b << "', " << std::endl; }

template<>
void DoMathOperation<bool, int>::div(bool, int b)
  { std::cerr << "cannot divide a boolean by the integer '" << b << "', " << std::endl; }

/// bool and bool
template<>
void DoMathOperation<bool, bool>::add(bool a, bool b)
  { ret = all_type(a || b); }

template<>
void DoMathOperation<bool, bool>::sub(bool a, bool b)
  { ret = all_type(a && b); }

template<>
void DoMathOperation<bool, bool>::mul(bool, bool)
  { std::cerr << "cannot multiply a boolean by a boolean, " << std::endl; }

template<>
void DoMathOperation<bool, bool>::div(bool, bool)
  { std::cerr << "cannot divide a boolean by a boolean, " << std::endl; }

/// bool and string
template<>
void DoMathOperation<bool, std::string>::add(bool, std::string b)
  { std::cerr << "cannot add the string '" << b << "' to a boolean, " << std::endl; }

template<>
void DoMathOperation<bool, std::string>::sub(bool, std::string b)
  { std::cerr << "cannot subtract the string '" << b << "' from a boolean, " << std::endl; }

template<>
void DoMathOperation<bool, std::string>::mul(bool, std::string b)
  { std::cerr << "cannot multiply a boolean by the string '" << b << "', " << std::endl; }

template<>
void DoMathOperation<bool, std::string>::div(bool, std::string b)
  { std::cerr << "cannot divide a boolean by the string '" << b << "', " << std::endl; }

/// string and int
template<>
void DoMathOperation<std::string, int>::add(std::string a, int b)
  { ret = all_type(a + std::to_string(b)); }

template<>
void DoMathOperation<std::string, int>::sub(std::string a, int b)
  { std::cerr << "cannot subtract the integer '" << b << "' from the string '" << a << "', " << std::endl; }

template<>
void DoMathOperation<std::string, int>::mul(std::string a, int b)
  {
    std::ostringstream ss;
    for (int i = 0; i < std::abs(b); ++i)
      ss << a;

    std::string str = ss.str();
    if (b < 0) // then we should reverse the string
      std::reverse(str.begin(), str.end());

    ret = str;
  }

template<>
void DoMathOperation<std::string, int>::div(std::string a, int b)
  { std::cerr << "cannot divide the string '" << a << "' by the integer '" << b << "', " << std::endl; }

/// string and bool
template<>
void DoMathOperation<std::string, bool>::add(std::string a, bool b)
  { ret = all_type(a + (b ? "true" : "false")); }

template<>
void DoMathOperation<std::string, bool>::sub(std::string a, bool)
  { std::cerr << "cannot subtract a bool from the string '" << a << "', " << std::endl; }

template<>
void DoMathOperation<std::string, bool>::mul(std::string a, bool)
  { std::cerr << "cannot multiply the string '" << a << "' by a bool, " << std::endl; }

template<>
void DoMathOperation<std::string, bool>::div(std::string a, bool)
  { std::cerr << "cannot divide the string '" << a << "' by a bool, " << std::endl; }

/// string and string
template<>
void DoMathOperation<std::string, std::string>::sub(std::string a, std::string b)
  { std::cerr << "cannot subtract the string '" << b << "' from the string '" << a << "', " << std::endl; }

template<>
void DoMathOperation<std::string, std::string>::mul(std::string a, std::string b)
  { std::cerr << "cannot multiply the string '" << a << "' by the string '" << b << "', " << std::endl; }

template<>
void DoMathOperation<std::string, std::string>::div(std::string a, std::string b)
  { std::cerr << "cannot divide the string '" << a << "' by the string '" << b << "', " << std::endl; }


// Gets the current type of both all_type variables and uses DoBoolOperation class to perform the mathematical operation and return the result
all_type mixed_mode_bool_filter(all_type a, all_type b, TokenType op)
{
  all_type_visitor atv = all_type_visitor();

  int_visitor iv = int_visitor();
  bool_visitor bv = bool_visitor();
  string_visitor sv = string_visitor();

  switch (boost::apply_visitor(atv, a))
  {
    case INT:
      switch (boost::apply_visitor(atv, b))
      {
        case INT:
          return DoBoolOperation<int, int>(a.apply_visitor(iv), b.apply_visitor(iv), op).get();

        case BOOL:
          std::cerr << "cannot relate the integer '" << a << "' with a boolean, " << std::endl;
          break;

        case STRING:
          std::cerr << "cannot relate the string '" << a << "' with a boolean, " << std::endl;
          break;
      }
      break;

    case BOOL:
      switch (boost::apply_visitor(atv, b))
      {
        case INT:
          std::cerr << "cannot relate a boolean with the integer '" << b << "', " << std::endl;
          break;

        case BOOL:
          return DoBoolOperation<bool, bool>(a.apply_visitor(bv), b.apply_visitor(bv), op).get();

        case STRING:
          std::cerr << "cannot relate a boolean with the string '" << b << "', " << std::endl;
          break;
      }
      break;

    case STRING:
      switch (boost::apply_visitor(atv, b))
      {
        case INT:
          std::cerr << "cannot relate the string '" << a << "'  with the integer '" << b << "', " << std::endl;
          break;

        case BOOL:
          std::cerr << "cannot relate the string '" << a << "'  with a boolean, " << std::endl;
          break;

        case STRING:
          return DoBoolOperation<std::string, std::string>(a.apply_visitor(sv), b.apply_visitor(sv), op).get();
      }
  }

  return all_type(0); // Won't reach here.
}

// Gets the current type of both all_type variables and uses DoMathOperation class to perform the boolean operation and return the result
all_type mixed_mode_math_filter(all_type a, all_type b, TokenType op)
{
  all_type_visitor atv = all_type_visitor();

  int_visitor iv = int_visitor();
  bool_visitor bv = bool_visitor();
  string_visitor sv = string_visitor();

  switch (boost::apply_visitor(atv, a))
  {
    case INT:
      switch (boost::apply_visitor(atv, b))
      {
        case INT:    return DoMathOperation<int, int>(a.apply_visitor(iv), b.apply_visitor(iv), op).get();
        case BOOL:   return DoMathOperation<int, bool>(a.apply_visitor(iv), b.apply_visitor(bv), op).get();
        case STRING: return DoMathOperation<int, std::string>(a.apply_visitor(iv), b.apply_visitor(sv), op).get();
      }

    case BOOL:
      switch (boost::apply_visitor(atv, b))
      {
        case INT:    return DoMathOperation<bool, int>(a.apply_visitor(bv), b.apply_visitor(iv), op).get();
        case BOOL:   return DoMathOperation<bool, bool>(a.apply_visitor(bv), b.apply_visitor(bv), op).get();
        case STRING: return DoMathOperation<bool, std::string>(a.apply_visitor(bv), b.apply_visitor(sv), op).get();
      }

    case STRING:
      switch (boost::apply_visitor(atv, b))
      {
        case INT:    return DoMathOperation<std::string, int>(a.apply_visitor(sv), b.apply_visitor(iv), op).get();
        case BOOL:   return DoMathOperation<std::string, bool>(a.apply_visitor(sv), b.apply_visitor(bv), op).get();
        case STRING: return DoMathOperation<std::string, std::string>(a.apply_visitor(sv), b.apply_visitor(sv), op).get();
      }
  }

  return all_type(0); // Won't reach here.
}

#endif // MIXEDMODEFILTER_H_INCLUDED
