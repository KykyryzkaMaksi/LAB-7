#include <iostream>
#include <cmath>
#include "OptimizationTask.h"
#include "OptimizationExceptions.h"
#include "ScopeLogger.h"

double targetFunction(double x) {
    return (x - 3.0) * (x - 3.0) + 2.0;
}

void level3(OptimizationTask& task) {
    ScopeLogger logger("level3");
    std::cout << "Attempting to solve with invalid tolerance (eps = -0.01)...\n";
    task.updateAndSolve(0.0, 5.0, -0.01, 1000, targetFunction);
}

void level2(OptimizationTask& task) {
    ScopeLogger logger("level2");
    try {
        level3(task);
    }
    catch (const OptimizationException& ex) {
        std::cout << "[LOG level2] Intercepted exception: " << ex.what() << '\n';
        throw; // Повторне генерування
    }
}

void level1(OptimizationTask& task) {
    ScopeLogger logger("level1");
    level2(task);
}

int main() {
    std::cout << "=== OptimizationLab Demo (Var 18) ===\n\n";

    try {
        OptimizationTask task(0.0, 5.0, 1e-4);
        task.solveMinimum(targetFunction);

        std::cout << "Minimum found at x = " << task.xExt() << ", f(x) = " << task.fExt() << "\n\n";

        std::cout << "--- Stack Unwinding Demo ---\n";
        level1(task);
    }
    catch (const InvalidToleranceException& ex) {
        std::cout << "[MAIN] Caught InvalidToleranceException! Value: " << ex.tolerance() << '\n';
    }
    catch (const std::exception& ex) {
        std::cout << "[MAIN] Caught std::exception: " << ex.what() << '\n';
    }

    return 0;
}