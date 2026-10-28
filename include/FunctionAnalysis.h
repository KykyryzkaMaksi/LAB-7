#pragma once

enum FA_Status {
    FA_OK = 0,
    FA_ERR_NULL_ARG = 1,
    FA_ERR_INVALID_INTERVAL = 2,
    FA_ERR_BAD_PARAM = 3,
    FA_ERR_NO_CONVERGENCE = 4,
    FA_ERR_NOT_FINITE = 5
};

using FA_Func = double (*)(double);

int fa_evaluate(FA_Func f, double x, double* result);
int fa_derivative(FA_Func f, double x, double h, double* result);
int fa_findMinimum(FA_Func f, double a, double b, double eps, int maxIter, double* xExt, double* fExt);
int fa_findMaximum(FA_Func f, double a, double b, double eps, int maxIter, double* xExt, double* fExt);