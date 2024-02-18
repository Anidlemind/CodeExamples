#include <iostream>

#include "../headers/driver.h"

int main(int argc, char** argv) {
  Driver driver;
  if (driver.parse(argv[1])) {
    return 1;
  }
  driver.program->execute();
  delete driver.program;
  driver.program = nullptr;
  return 0;
}