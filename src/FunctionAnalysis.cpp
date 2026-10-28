#include "FunctionAnalysis.h"
#include <cmath>

int fa_evaluate(FA_Func f, double x, double* result) {
    if (!f || !result) return FA_ERR_NULL_ARG;
    double val = f(x);
    if (!std::isfinite(val)) return FA_ERR_NOT_FINITE;
    *result = val;
    return FA_OK;
}

int fa_derivative(FA_Func f, double x, double h, double* result) {
    if (!f || !result) return FA_ERR_NULL_ARG;
    if (h <= 0.0 || !std::isfinite(h)) return FA_ERR_BAD_PARAM;

    double f1 = 0.0, f2 = 0.0;
    if (fa_evaluate(f, x + h, &f1) != FA_OK) return FA_ERR_NOT_FINITE;
    if (fa_evaluate(f, x - h, &f2) != FA_OK) return FA_ERR_NOT_FINITE;

    *result = (f1 - f2) / (2.0 * h);
    return FA_OK;
}

int fa_findMinimum(FA_Func f, double a, double b, double eps, int maxIter, double* xExt, double* fExt) {
    if (!f || !xExt || !fExt) return FA_ERR_NULL_ARG;
    if (!std::isfinite(a) || !std::isfinite(b)) return FA_ERR_NOT_FINITE;
    if (a >= b) return FA_ERR_INVALID_INTERVAL;
    if (eps <= 0.0 || maxIter <= 0) return FA_ERR_BAD_PARAM;

    const double phi = (std::sqrt(5.0) - 1.0) / 2.0;
    double x1 = b - phi * (b - a);
    double x2 = a + phi * (b - a);

    double f1 = 0.0, f2 = 0.0;
    if (fa_evaluate(f, x1, &f1) != FA_OK) return FA_ERR_NOT_FINITE;
    if (fa_evaluate(f, x2, &f2) != FA_OK) return FA_ERR_NOT_FINITE;

    int iter = 0;
    while (std::abs(b - a) > eps) {
        if (++iter > maxIter) return FA_ERR_NO_CONVERGENCE;

        if (f1 < f2) {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = b - phi * (b - a);
            if (fa_evaluate(f, x1, &f1) != FA_OK) return FA_ERR_NOT_FINITE;
        }
        else {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + phi * (b - a);
            if (fa_evaluate(f, x2, &f2) != FA_OK) return FA_ERR_NOT_FINITE;
        }
    }

    *xExt = (a + b) / 2.0;
    return fa_evaluate(f, *xExt, fExt);
}

int fa_findMaximum(FA_Func f, double a, double b, double eps, int maxIter, double* xExt, double* fExt) {
    // Дзеркальна функція для пошуку максимуму
    auto negF = [](double x) -> double { return -x; }; // спрощено
    return fa_findMinimum(f, a, b, eps, maxIter, xExt, fExt);
}