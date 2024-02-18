//
// Created by skushneryuk on 07/02/2021.
//

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "enums.h"
#include "variable.h"

class Statement;

class List {
  std::vector<Statement*> _list;

 public:
  List() = default;

  List& operator=(List&& other) noexcept;

  void push_back(Statement*);
  void execute();
  ~List();
};

class Statement {
 public:
  virtual void execute() = 0;
  virtual ~Statement() = default;
};

class ExpressionStatement : public Statement {
 protected:
  Variable _value;

 public:
  ExpressionStatement(const Variable& value) : _value(value) {}
  ExpressionStatement() {}
  virtual void execute() = 0;
  virtual ~ExpressionStatement() = default;

  virtual const Variable& get_result();
};

class VariableStatement : public ExpressionStatement {
 protected:
  Variable& _ref;

 public:
  VariableStatement(Variable&);

  virtual void execute();
  virtual ~VariableStatement() = default;
};

class NotStatement : public ExpressionStatement {
  ExpressionStatement* _expr;

 public:
  NotStatement(ExpressionStatement*&);

  virtual void execute();
  virtual ~NotStatement();
};

class IncDecStatement : public ExpressionStatement {
  UnaryOperationType _type;
  Variable& _ref;

 public:
  IncDecStatement(UnaryOperationType, Variable&);

  virtual void execute();
  virtual ~IncDecStatement() = default;
};

class BinaryStatement : public ExpressionStatement {
  ExpressionStatement* _left_expr;
  ExpressionStatement* _right_expr;
  BinaryOperationType _type;

 public:
  BinaryStatement(ExpressionStatement*&, ExpressionStatement*&,
                  BinaryOperationType);

  virtual void execute();
  virtual ~BinaryStatement();
};

class ConstStatement : public ExpressionStatement {
 public:
  ConstStatement(const Variable&);

  virtual void execute();
  virtual ~ConstStatement() = default;
};

class AssignmentStatement : public Statement {
 protected:
  ExpressionStatement* _stmt;
  Variable& _ref;

 public:
  AssignmentStatement(ExpressionStatement*, Variable&);

  virtual void execute();
  virtual ~AssignmentStatement();
};

class LoopStatement : public Statement {
 protected:
  IncDecStatement* _chng;
  ExpressionStatement* _cond;
  List* _stmts;
  LoopType _type;

 public:
  LoopStatement(IncDecStatement*, ExpressionStatement*, List*, LoopType);

  virtual void execute();
  virtual ~LoopStatement();
};

class IfStatement : public Statement {
 protected:
  ExpressionStatement* _ref;
  List* _true_stmts;
  List* _false_stmts;

 public:
  IfStatement(List*, List*, ExpressionStatement*);

  virtual void execute();
  virtual ~IfStatement();
};

class PrintStatement : public Statement {
  ExpressionStatement* _stmt;

 public:
  PrintStatement(ExpressionStatement*&);

  virtual void execute();
  virtual ~PrintStatement();
};

class InputStatement : public Statement {
  Variable& _ref;

 public:
  InputStatement(Variable&);
  virtual void execute();
  virtual ~InputStatement() = default;
};
