#include "../headers/variable.h"

Variable::Variable(Type type) : _type(type) {
  if (_type == Type::NUMBR) {
    _value = int();
  }
  if (_type == Type::YARN) {
    _value = std::string();
  }
  if (_type == Type::TROOF) {
    _value = bool();
  }
}

bool Variable::to_bool() const {
  if (_type == Type::NUMBR) {
    // std::cout << "I'm a NUM\n";
    return std::get<int>(_value);
  } else if (_type == Type::YARN) {
    // std::cout << "I'm a YARN\n";
    return !std::get<std::string>(_value).empty();
  } else if (_type == Type::TROOF) {
    // std::cout << "I'm a TROOF\n";
    return std::get<bool>(_value);
  }
  throw std::bad_cast();
}

std::string Variable::to_string() const {
  if (_type == Type::NUMBR) {
    return std::to_string(std::get<int>(_value));
  } else if (_type == Type::YARN) {
    return std::get<std::string>(_value);
  } else if (_type == Type::TROOF) {
    return bool_to_string(std::get<bool>(_value));
  }
  throw std::bad_cast();
}

int Variable::to_int() const {
  if (_type == Type::NUMBR) {
    return std::get<int>(_value);
  } else if (_type == Type::YARN) {
    return std::stoi(std::get<std::string>(_value));
  } else if (_type == Type::TROOF) {
    return std::get<bool>(_value);
  }
  throw std::bad_cast();
}

Variable Variable::conc(const Variable& other) const {
  if (_type == other._type) {
    return Variable(to_string() + other.to_string());
  }
  throw std::bad_cast();
}

Variable Variable::operator+(const Variable& other) const {
  if (_type == other._type) {
    return Variable(to_int() + other.to_int());
  }
  throw std::bad_cast();
}

Variable Variable::operator-(const Variable& other) const {
  if (_type == other._type && _type == Type::NUMBR) {
    return Variable(to_int() - other.to_int());
  }
  throw std::bad_cast();
}

Variable Variable::operator*(const Variable& other) const {
  if (_type == other._type && _type == Type::NUMBR) {
    return Variable(to_int() * other.to_int());
  }
  throw std::bad_cast();
}

Variable Variable::operator/(const Variable& other) const {
  if (_type == other._type && _type == Type::NUMBR) {
    return Variable(to_int() / other.to_int());
  }
  throw std::bad_cast();
}

Variable Variable::operator%(const Variable& other) const {
  if (_type == other._type && _type == Type::NUMBR) {
    return Variable(to_int() % other.to_int());
  }
  throw std::bad_cast();
}

Variable Variable::operator&(const Variable& other) const {
  return Variable(to_bool() & other.to_bool());
}

Variable Variable::operator^(const Variable& other) const {
  return Variable(to_bool() ^ other.to_bool());
}

Variable Variable::operator|(const Variable& other) const {
  return Variable(to_bool() | other.to_bool());
}

Variable& Variable::operator++() {
  if (_type != Type::NUMBR) {
    throw std::bad_cast();
  }
  _value = to_int() + 1;
  return *this;
}

Variable& Variable::operator--() {
  if (_type != Type::NUMBR) {
    throw std::bad_cast();
  }
  _value = to_int() - 1;
  return *this;
}

Variable Variable::operator!() const { return Variable(!to_bool()); }

bool Variable::operator==(const Variable& other) const {
  if (_type != other._type) {
    throw std::bad_cast();
  }
  return _value == other._value;
}

bool Variable::operator!=(const Variable& other) const {
  if (_type != other._type) {
    throw std::bad_cast();
  }
  return !(*this == other);
}

Variable max(const Variable& first, const Variable& second) {
  if (first._type != second._type) {
    throw std::bad_cast();
  }
  return Variable(std::max(first.to_int(), second.to_int()));
}

Variable min(const Variable& first, const Variable& second) {
  if (first._type != second._type) {
    throw std::bad_cast();
  }
  return Variable(std::min(first.to_int(), second.to_int()));
}