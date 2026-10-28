#include "OptimizationTask.h"
#include "OptimizationExceptions.h"
#include <cmath>
#include <stdexcept>

void OptimizationTask::validateParams(double a, double b, double eps, int maxIter) const {
    if (!std::isfinite(a) || !std::isfinite(b)) {
        throw NonFiniteValueException();
    }
    if (a >= b) {
        throw std::invalid_argument("Left boundary 'a' must be strictly less than 'b'");
    }
    if (eps <= 0.0 || maxIter <= 0) {
        throw InvalidToleranceException(eps);
    }
}

OptimizationTask::OptimizationTask(double a, double b, double eps, int maxIter)
    : a_(a), b_(b), eps_(eps), maxIter_(maxIter), xExt_(0.0), fExt_(0.0), hasResult_(false) {
    validateParams(a, b, eps, maxIter);
}

double OptimizationTask::a() const noexcept { return a_; }
double OptimizationTask::b() const noexcept { return b_; }
double OptimizationTask::eps() const noexcept { return eps_; }
int OptimizationTask::maxIter() const noexcept { return maxIter_; }
bool OptimizationTask::hasResult() const noexcept { return hasResult_; }

double OptimizationTask::xExt() const {
    if (!hasResult_) throw std::out_of_range("No optimization result available");
    return xExt_;
}

double OptimizationTask::fExt() const {
    if (!hasResult_) throw std::out_of_range("No optimization result available");
    return fExt_;
}

void OptimizationTask::solveMinimum(FA_Func f) {
    if (!f) throw std::invalid_argument("Function pointer cannot be null");

    double xRes = 0.0, fRes = 0.0;
    int status = fa_findMinimum(f, a_, b_, eps_, maxIter_, &xRes, &fRes);

    switch (status) {
    case FA_OK:
        xExt_ = xRes;
        fExt_ = fRes;
        hasResult_ = true;
        break;
    case FA_ERR_NULL_ARG:
        throw std::invalid_argument("Null pointer argument passed");
    case FA_ERR_INVALID_INTERVAL:
        throw std::invalid_argument("Invalid interval [a, b]");
    case FA_ERR_BAD_PARAM:
        throw InvalidToleranceException(eps_);
    case FA_ERR_NO_CONVERGENCE:
        throw ConvergenceException(maxIter_);
    case FA_ERR_NOT_FINITE:
        throw NonFiniteValueException();
    default:
        throw OptimizationException("Unknown optimization error");
    }
}

// ПРИНЦИП PREPARE-THEN-COMMIT (Сильна гарантія)
void OptimizationTask::updateAndSolve(double newA, double newB, double newEps, int newMaxIter, FA_Func f) {
    // 1. Створюємо тимчасовий об'єкт і рахуємо результат
    OptimizationTask temp(newA, newB, newEps, newMaxIter);
    temp.solveMinimum(f);

    // 2. Оновлюємо стан поточного об'єкта лише після успіху
    a_ = temp.a_;
    b_ = temp.b_;
    eps_ = temp.eps_;
    maxIter_ = temp.maxIter_;
    xExt_ = temp.xExt_;
    fExt_ = temp.fExt_;
    hasResult_ = temp.hasResult_;
}

void OptimizationTask::updateAndSolveUnsafe(double newA, double newB, double newEps, int newMaxIter, FA_Func f) {
    // Змінюємо початковий стан ДО завершення обчислень
    a_ = newA;
    b_ = newB;
    eps_ = newEps;
    maxIter_ = newMaxIter;
    hasResult_ = false;

    solveMinimum(f);
}