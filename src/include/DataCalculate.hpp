#ifndef DATACALC_H
#define DATACALC_H

#include <memory>
#include "mathfunc.h"

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
    Logger::Instance().LogWarn("Operation not set or invalid");
    throw std::invalid_argument("Unknown operation: " + op);
  }
};
#endif