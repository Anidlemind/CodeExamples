#include "../headers/driver.h"

Driver::Driver()
    : trace_parsing(false),
      trace_scanning(false),
      program(nullptr),
      scanner(*this),
      parser(scanner, *this) {}

int Driver::parse(const std::string& f) {
  file = f;
  location.initialize(&file);
  scan_begin();
  parser.set_debug_level(trace_parsing);
  int res = parser();
  scan_end();
  return res;
}

void Driver::scan_begin() {
  scanner.set_debug(trace_scanning);
  if (!(file.empty() || file == "-")) {
    stream.open(file);
    scanner.yyrestart(&stream);
  }
}

void Driver::scan_end() { stream.close(); }
