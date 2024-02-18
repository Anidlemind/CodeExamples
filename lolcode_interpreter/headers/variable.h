#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <variant>

#include "enums.h"

class Variable {
 private:
  std::variant<int, bool, std::string> _value;
  Type _type;

 public:
  //template <typename T>
  // T get_value() const {
  //   return std::get<T>(_value);
  // }
  // Type get_type() const {
  //   return _type;
  // }
  explicit Variable(Type type = Type::NUMBR);
  Variable(const Variable&) = default;

  Variable& operator=(const Variable& other) = default;
  Variable& operator=(Variable&& other) = default;

  Variable conc(const Variable&) const;
  Variable operator+(const Variable&) const;
  Variable operator-(const Variable&) const;
  Variable operator*(const Variable&) const;
  Variable operator/(const Variable&) const;
  Variable operator%(const Variable&) const;
  Variable operator&(const Variable&) const;
  Variable operator^(const Variable&) const;
  Variable operator|(const Variable&) const;
  Variable operator!() const;

  Variable& operator++();
  Variable& operator--();

  bool operator==(const Variable&) const;
  bool operator!=(const Variable&) const;

  explicit Variable(const std::string& value) : _type(Type::YARN), _value(value) {}
  explicit Variable(const int& value) : _type(Type::NUMBR), _value(value) {}
  explicit Variable(const bool& value) : _type(Type::TROOF), _value(value) {}

  bool to_bool() const;
  std::string to_string() const;
  int to_int() const;

  friend Variable max(const Variable&, const Variable&);
  friend Variable min(const Variable&, const Variable&);
};

Variable max(const Variable&, const Variable&);

Variable min(const Variable&, const Variable&);