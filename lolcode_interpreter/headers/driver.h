#pragma once

#include <fstream>
#include <map>
#include <string>

#include "../parser.hh"
#include "scanner.h"
#include "statement.h"
#include "variable.h"

class Driver {
 public:
  Driver();
  std::map<std::string, Variable> variables;
  int result;  
  int parse(const std::string& f);
  std::string file;
  bool trace_parsing;
  
  void scan_begin();
  void scan_end();
  
  bool trace_scanning;
  yy::location location;

  friend class Scanner;
  Scanner scanner;
  yy::parser parser;

  List* program;
 private:
  std::ifstream stream;
};
