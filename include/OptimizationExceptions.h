#pragma once
#include <stdexcept>
#include <string>

// Базовий клас винятків бібліотеки
class OptimizationException : public std::runtime_error {
public:
    explicit OptimizationException(const std::string& message)
        : std::runtime_error(message) {}
};

// 1. Помилка: некоректна точність або параметри
class InvalidToleranceException : public OptimizationException {
private:
    double tolerance_;
public:
    explicit InvalidToleranceException(double tol)
        : OptimizationException("Invalid tolerance or maxIter parameter: " + std::to_string(tol)), tolerance_(tol) {
    }

    double tolerance() const noexcept { return tolerance_; }
};

// 2. Помилка: NaN або Inf під час обчислень
class NonFiniteValueException : public OptimizationException {
public:
    NonFiniteValueException()
        : OptimizationException("Function or interval value is non-finite (NaN or Inf)") {
    }
};

// 3. Помилка: відсутність збіжності алгоритму
class ConvergenceException : public OptimizationException {
private:
    int maxIter_;
public:
    explicit ConvergenceException(int maxIter)
        : OptimizationException("Algorithm failed to converge within " + std::to_string(maxIter) + " iterations"), maxIter_(maxIter) {
    }

    int maxIter() const noexcept { return maxIter_; }
};