#ifndef MATHFUNC_H
#define MATHFUNC_H
#include <climits>
#include <vector>
#include <stdexcept>

namespace math{
class BaseMathClass
{
    //fnum, snum, result
    public:
    virtual ~BaseMathClass() = default;
    virtual int calculate(const std::vector<int>& dataSet) const = 0;
};

class SumMath: public BaseMathClass
{
    public:
    int calculate(const std::vector<int>& dataSet) const override
    {
    if (((dataSet.at(1) > 0) && (dataSet.at(0) > INT_MAX - dataSet.at(1))) ||
        ((dataSet.at(1) < 0) && (dataSet.at(0) < INT_MIN - dataSet.at(1))))
        throw std::invalid_argument("invalid input");
    return dataSet.at(0) + dataSet.at(1);
    }
};

class SubMath: public BaseMathClass
{
    public:
    inline int calculate(const std::vector<int>& dataSet) const override
    {
        if ((dataSet.at(1) < 0 && dataSet.at(0) > INT_MAX + dataSet.at(1)) ||
        (dataSet.at(1) > 0 && dataSet.at(0) < INT_MIN + dataSet.at(1)))
        throw std::invalid_argument("invalid input");
        return dataSet.at(0) - dataSet.at(1);
    }
};

class MulMath: public BaseMathClass
{
    public:
    inline int calculate(const std::vector<int>& dataSet) const override
    {
    if (dataSet.at(0) > 0) {
        if (dataSet.at(1) > 0 && dataSet.at(0) > INT_MAX / dataSet.at(1)) throw std::invalid_argument("invalid input");
        if (dataSet.at(1) < 0 && dataSet.at(1) < INT_MIN / dataSet.at(0)) throw std::invalid_argument("invalid input");
    }

    if (dataSet.at(0) < 0) {
        if (dataSet.at(1) > 0 && dataSet.at(0) < INT_MIN / dataSet.at(1)) throw std::invalid_argument("invalid input");
        if (dataSet.at(1) < 0 && dataSet.at(0) < INT_MAX / dataSet.at(1)) throw std::invalid_argument("invalid input");
    }
    return dataSet.at(0) * dataSet.at(1);
    }
};

class DivMath: public BaseMathClass
{
    public:
    inline int calculate(const std::vector<int>& dataSet) const override
    {
    if (dataSet.at(0) == 0) throw  std::runtime_error("division by zero");;
    if (dataSet.at(0) == INT_MIN && dataSet.at(1) == -1) throw std::invalid_argument("invalid input");
    return dataSet.at(0) / dataSet.at(1);
    }
};

class MaxMath: public BaseMathClass
{
    public:
    inline int calculate(const std::vector<int>& dataSet) const override
    {
    if (dataSet.at(1) < 0) throw std::invalid_argument("invalid input");

    int result = 1;

    for (int i = 0; i < dataSet.at(1); i++)
    {
        if (dataSet.at(0) != 0 && result > INT_MAX / dataSet.at(0)) throw std::invalid_argument("invalid input");

        if (dataSet.at(0) != 0 && result < INT_MIN / dataSet.at(0)) throw std::invalid_argument("invalid input");

        result = result * dataSet.at(0);
    }
    return result;
    }

};

class FactMath : public BaseMathClass {
 public:
  int calculate(const std::vector<int>& dataSet) const override {
    if (dataSet.size() != 1) throw std::invalid_argument("invalid input");
    int n = dataSet.at(0);
    if (n < 0) throw std::invalid_argument("invalid input");
    if (n > 12) throw std::overflow_error("Factorial overflow");
    return factorialHelper(n);
  }

 private:
  int factorialHelper(int n) const {
    if (n == 0 || n == 1) return 1;
    int prev = factorialHelper(n - 1);
    if (prev > INT_MAX / n) throw std::overflow_error("Factorial overflow");
    return n * prev;
  }
};
}
#endif
