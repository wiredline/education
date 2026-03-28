#include "./include/mathfunc.h"
#include <iostream>
#include <limits>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

class JsonParse {
private:
  std::string m_input;
  std::string m_op;
  std::vector<int> m_DataSet;

public:
  JsonParse() {}
  void Parse(const std::string &input) {
    using json = nlohmann::json;
    try {
      m_input = input;
      json j = json::parse(m_input);
      if (!j.contains("operation") || !j.contains("args"))
        throw std::invalid_argument("Missing required fields");
      m_op = j.at("operation").get<std::string>();
      m_DataSet = j.at("args").get<std::vector<int>>();

    } catch (const json::parse_error &e) {
      std::cerr << "JSON parse error: " << e.what() << "\n";
      throw;
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << "\n";
      throw;
    }
  }
  const std::string &GetOperation() const { return m_op; }
  const std::vector<int> &GetArgs() const { return m_DataSet; }
};

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
      int a = args[0], b = args[1];
      if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b)) {
        throw std::overflow_error("Subtraction overflow");
      }
    }

    if (op == "*") {
      int a = args[0], b = args[1];

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
      int a = args[0], b = args[1];

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
      int n = args[0];

      if (n < 0) {
        throw std::invalid_argument("Factorial of negative number");
      }

      if (n > 12) { // для int
        throw std::overflow_error("Factorial overflow");
      }
    }
  }
};

class DataCalculate {
public:
  static std::unique_ptr<math::BaseMathClass> Create(const std::string &op) {
    if (op == "+")
      return std::make_unique<math::SumMath>();
    if (op == "-")
      return std::make_unique<math::SubMath>();
    if (op == "*")
      return std::make_unique<math::MulMath>();
    if (op == "/")
      return std::make_unique<math::DivMath>();
    if (op == "^")
      return std::make_unique<math::MaxMath>();
    if (op == "!")
      return std::make_unique<math::FactMath>();
    throw std::invalid_argument("Unknown operation: " + op);
  }
};

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

class Runner {
private:
  std::vector<int> DataSet{1, 2, 43}; // fnum, snum
  std::string m_operation;
  std::string m_input;
  int result;

public:
  explicit Runner(const std::string &input) : m_input(input) {
    // Parser
    JsonParse parser;
    parser.Parse(m_input);
    DataSet = parser.GetArgs();
    m_operation = parser.GetOperation();

    // Checker
    JsonCheck checker;
    try {
      checker.Validate(m_operation, DataSet);
    } catch (const std::exception &exep) {
      std::cout << "Validation is not true " << exep.what();
      throw;
    }

    // Calculate
    auto operation = DataCalculate::Create(m_operation);
    result = operation->calculate(DataSet);

    // Printable
    Printer print(result);
    print.PrintToCMD();
  }
  const std::string &GetOperation() const { return m_operation; }
  const std::vector<int> &GetArgs() const { return DataSet; }
};

int main(int argc, char *argv[]) //'{"operation": "+", "args": [3, 5]}'
{
  // help
  try {
    if (argc == 1 || std::string(argv[1]) == "-h") {
      printf("This program use for calculation\n");
      printf("How to use:\n");
      printf("Can addition(+), subtraction(-), multiplication(*), division(/), "
             "exponentiation(^), factorial(!)\n");
      printf("you need to insert json \n");
      printf("'{\"operation\": \"+\", \"args\": [3, 5]}'");
      return 0;
    }
    if (argc < 2)
      throw std::runtime_error("not loaded json");
    // start
    std::string input = argv[1];
   // input = "{\"operation\": \"+\", \"args\": [3, 5]}";
    std::cout << "INPUT: " << input << std::endl;
    Runner run(input);
  } catch (std::exception &error) {
    std::cout << error.what();
  }
  return 0;
}