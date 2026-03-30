#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

class Printer {
private:
  int m_result;

public:
  Printer(int b) : m_result(b) {}
  void PrintToCMD() const {
    std::cout << "Result of calculating is: ";
    std::cout << m_result << std::endl;
  }
};
#endif