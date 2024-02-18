#include "../headers/statement.h"

void List::execute() {
  for (auto statement : _list) {
    statement->execute();
  }
}

List::~List() {
  for (auto statement : _list) {
    delete statement;
  }
}

List& List::operator=(List&& other) noexcept {
  _list = std::move(other._list);
  other._list.clear();
  return *this;
}

void List::push_back(Statement* stmt) { _list.push_back(stmt); }

const Variable& ExpressionStatement::get_result() { return _value; }

VariableStatement::VariableStatement(Variable& ref) : _ref(ref) {}

void VariableStatement::execute() { _value = _ref; }

NotStatement::NotStatement(ExpressionStatement*& expr) : _expr(expr) {}

void NotStatement::execute() {
  _expr->execute();
  _value = !(_expr->get_result());
}

NotStatement::~NotStatement() { delete _expr; }

IncDecStatement::IncDecStatement(UnaryOperationType type, Variable& ref)
    : _type(type), _ref(ref) {}

void IncDecStatement::execute() {
  if (_type == UnaryOperationType::INC) {
    ++_ref;
    return;
  }
  --_ref;
}

BinaryStatement::BinaryStatement(ExpressionStatement*& left,
                                 ExpressionStatement*& right,
                                 BinaryOperationType type)
    : _left_expr(left), _right_expr(right), _type(type) {}

void BinaryStatement::execute() {
  _left_expr->execute();
  _right_expr->execute();
  switch (_type) {
    case BinaryOperationType::EQUALS:
      // if (_left_expr->get_result().get_type() == Type::NUMBR) {
      //   std::cout << _left_expr->get_result().get_value<int>() << '\n';
      // }
      // if (_left_expr->get_result().get_type() == Type::TROOF) {
      //   std::cout << _left_expr->get_result().get_value<bool>() << '\n';
      // }
      // if (_left_expr->get_result().get_type() == Type::YARN) {
      //   std::cout << _left_expr->get_result().get_value<std::string>() << '\n';
      // }
      _value = Variable(_left_expr->get_result() == _right_expr->get_result());
      break;
    case BinaryOperationType::DIFFER:
      _value = Variable(_left_expr->get_result() != _right_expr->get_result());
      break;
    case BinaryOperationType::SUM:
      _value = Variable(_left_expr->get_result() + _right_expr->get_result());
      break;
    case BinaryOperationType::SUB:
      _value = Variable(_left_expr->get_result() - _right_expr->get_result());
      break;
    case BinaryOperationType::MUL:
      _value = Variable(_left_expr->get_result() * _right_expr->get_result());
      break;
    case BinaryOperationType::DIV:
      _value = Variable(_left_expr->get_result() / _right_expr->get_result());
      break;
    case BinaryOperationType::MOD:
      _value = Variable(_left_expr->get_result() % _right_expr->get_result());
      break;
    case BinaryOperationType::AND:
      _value = Variable(_left_expr->get_result() & _right_expr->get_result());
      break;
    case BinaryOperationType::XOR:
      _value = Variable(_left_expr->get_result() ^ _right_expr->get_result());
      break;
    case BinaryOperationType::OR:
      _value = Variable(_left_expr->get_result() | _right_expr->get_result());
      break;
    case BinaryOperationType::CONC:
      _value = Variable(_left_expr->get_result().conc(_right_expr->get_result()));
      break;
    case BinaryOperationType::MAX:
      _value = Variable(max(_left_expr->get_result(), _right_expr->get_result()));
      break;
    case BinaryOperationType::MIN:
      _value = Variable(min(_left_expr->get_result(), _right_expr->get_result()));
      break;
    default:
      break;
  }
}

BinaryStatement::~BinaryStatement() {
  delete _left_expr;
  delete _right_expr;
}

ConstStatement::ConstStatement(const Variable& value)
    : ExpressionStatement(value) {}

void ConstStatement::execute() {}

AssignmentStatement::AssignmentStatement(ExpressionStatement* stmt,
                                         Variable& ref)
    : _stmt(stmt), _ref(ref) {}

void AssignmentStatement::execute() {
  _stmt->execute();
  _ref = _stmt->get_result();
}

AssignmentStatement::~AssignmentStatement() { delete _stmt; }

LoopStatement::LoopStatement(IncDecStatement* step,
                             ExpressionStatement* condition, List* statements,
                             LoopType type)
    : _chng(step), _cond(condition), _stmts(statements), _type(type) {}

void LoopStatement::execute() {
  _cond->execute();

  while (_cond->get_result().to_bool() ^ (_type == LoopType::TILL)) {
    _stmts->execute();
    _chng->execute();
    _cond->execute();
  }
}

LoopStatement::~LoopStatement() {
  delete _chng;
  delete _cond;
  delete _stmts;
}

IfStatement::IfStatement(List* true_stmts, List* false_stmts, ExpressionStatement* ref)
    : _ref(ref), _true_stmts(true_stmts), _false_stmts(false_stmts) {}

void IfStatement::execute() {
  // if (_ref->get_result().get_type() == Type::NUMBR) {
  //   std::cout << "NUM???\n";
  // }
  // if (_ref->get_result().get_type() == Type::YARN) {
  //   std::cout << "YARN???\n";
  // }
  _ref->execute();
  if (_ref->get_result().to_bool()) {
    _true_stmts->execute();
    return;
  }
  _false_stmts->execute();
}

IfStatement::~IfStatement() {
  delete _true_stmts;
  delete _false_stmts;
}

PrintStatement::PrintStatement(ExpressionStatement*& stmt) : _stmt(stmt) {}

void PrintStatement::execute() {
  _stmt->execute();
  std::cout << _stmt->get_result().to_string() << '\n';
}

PrintStatement::~PrintStatement() { delete _stmt; }

InputStatement::InputStatement(Variable& ref) : _ref(ref) {}

void InputStatement::execute() {
  std::string str;
  std::cin >> str;
  _ref = Variable(str);
}
