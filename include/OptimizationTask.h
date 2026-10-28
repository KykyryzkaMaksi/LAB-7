#pragma once
#include "FunctionAnalysis.h"
#include <cstddef>

class OptimizationTask {
private:
    double a_;
    double b_;
    double eps_;
    int maxIter_;
    double xExt_;
    double fExt_;
    bool hasResult_;

    void validateParams(double a, double b, double eps, int maxIter) const;

public:
    OptimizationTask(double a, double b, double eps, int maxIter = 1000);

    double a() const noexcept;
    double b() const noexcept;
    double eps() const noexcept;
    int maxIter() const noexcept;
    bool hasResult() const noexcept;

    double xExt() const;
    double fExt() const;

    void solveMinimum(FA_Func f);

    // БАГАТОКРОКОВА ОПЕРАЦІЯ (Сильна гарантія / Strong Guarantee)
    void updateAndSolve(double newA, double newB, double newEps, int newMaxIter, FA_Func f);

    // Навмисно некоректна реалізація (для демонстрації виявлення помилки тестом)
    void updateAndSolveUnsafe(double newA, double newB, double newEps, int newMaxIter, FA_Func f);
};