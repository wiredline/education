#ifndef JSONCHECK_H
#define JSONCHECK_H

#include <stdexcept>
#include <string>
#include <climits>
#include <vector>

class JsonCheck {
public:
  void Validate(const std::string &op, const std::vector<int> &args) {
    if (op != "+" && op != "-" && op != "*" && op != "/" && op != "^" &&
        op != "!") {
      throw std::invalid_argument("Unknown operation");
    }
    if (op == "!") {
      if (args.size() != 1) {
        throw std::invalid_argument("Factorial requires 1 argument");
      }
    } else {
      if (args.size() != 2) {
        throw std::invalid_argument("Binary operation requires 2 arguments");
      }
    }

    if (op == "+") {
      int a = args.at(0), b = args.at(1);
      if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        throw std::overflow_error("Addition overflow");
      }
    }

    if (op == "-") {
      int a = args.at(0), b = args.at(1);
      if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b)) {
        throw std::overflow_error("Subtraction overflow");
      }
    }

    if (op == "*") {
      int a = args.at(0), b = args.at(1);

      if (a != 0 && b != 0) {
        if (a == -1 && b == INT_MIN) {
          throw std::overflow_error("Multiplication overflow");
        }
        if (b == -1 && a == INT_MIN) {
          throw std::overflow_error("Multiplication overflow");
        }

        if (a > 0 && b > 0 && a > INT_MAX / b) {
          throw std::overflow_error("Multiplication overflow");
        }
        if (a > 0 && b < 0 && b < INT_MIN / a) {
          throw std::overflow_error("Multiplication overflow");
        }
        if (a < 0 && b > 0 && a < INT_MIN / b) {
          throw std::overflow_error("Multiplication overflow");
        }
        if (a < 0 && b < 0 && a < INT_MAX / b) {
          throw std::overflow_error("Multiplication overflow");
        }
      }
    }

    if (op == "/") {
      int a = args.at(0), b = args.at(1);

      if (b == 0) {
        throw std::runtime_error("Division by zero");
      }

      if (a == INT_MIN && b == -1) {
        throw std::overflow_error("Division overflow");
      }
    }

    if (op == "^") {
      
      int base = args[0], exp = args[1];

      if (exp < 0) {
        throw std::invalid_argument("Negative exponent not supported");
      }

      int result = 1;
      for (int i = 0; i < exp; ++i) {
        if (base != 0 && result > INT_MAX / base) {
          throw std::overflow_error("Power overflow");
        }
        result *= base;
      }
    }

    if (op == "!") {
      int n = args.at(0);

      if (n < 0) {
        throw std::invalid_argument("Factorial of negative number");
      }

      if (n > 12) { // для int
        throw std::overflow_error("Factorial overflow");
      }
    }
  }
};

#endif