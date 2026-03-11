#ifndef MATHFUNC_H
#define MATHFUNC_H
#include <limits.h>

namespace math{
void sum(int &a, int &b, int &c) //суммирование a + b
{
    if (((b > 0) && (a > INT_MAX - b)) ||
        ((b < 0) && (a < INT_MIN - b))) throw -1;
 c = a + b;
}

inline void sub(int &a, int &b, int &c) // вычитание
{
    if ((b < 0 && a > INT_MAX + b) ||
        (b > 0 && a < INT_MIN + b))  throw -1;
c = a - b;
}

inline void mul(int &a, int &b, int &c) // умножение
{
    if (a > 0) {
        if (b > 0 && a > INT_MAX / b) throw -1;
        if (b < 0 && b < INT_MIN / a) throw -1;
    }

    if (a < 0) {
        if (b > 0 && a < INT_MIN / b) throw -1;
        if (b < 0 && a < INT_MAX / b) throw -1;
    }
c = a * b;
}

inline void div(int &a, int &b, int &c) // деление
{
    if (b == 0) throw -1.1;
    if (a == INT_MIN && b == -1) throw -1;
c = a / b;
}

inline void max(int &base, int &exp, int &c) //возведение в степень
{
    if (exp < 0) throw -1;

    int result = 1;

    for (int i = 0; i < exp; i++)
    {
        if (base != 0 && result > INT_MAX / base) throw -1;

        if (base != 0 && result < INT_MIN / base) throw -1;

        result = result * base;
    }
    c = result;

}

inline int fact(int n) // факториал
{
    if(n > 12) throw -1;
    if(n < 0) throw -1;

    if (n < 0)
        return -1;

    if (n == 0 || n == 1)
        return 1;

    int prev = fact(n - 1);

    if (prev == -1)
        return -1;

    if (prev > INT_MAX / n)
        return -1;

    return n * prev;
}
}
#endif