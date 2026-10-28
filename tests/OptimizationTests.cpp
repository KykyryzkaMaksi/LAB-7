#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>
#include "OptimizationTask.h"
#include "OptimizationExceptions.h"

double testSquareFunc(double x) {
    return (x - 2.0) * (x - 2.0); // x = 2.0, f(x) = 0.0
}

double testNonFiniteFunc(double) {
    return std::numeric_limits<double>::quiet_NaN();
}

// 1. Перевірка звичайного розрахунку
TEST(OptimizationTaskTest, FindsMinimumCorrectly) {
    OptimizationTask task(0.0, 4.0, 1e-4);
    task.solveMinimum(testSquareFunc);

    EXPECT_NEAR(task.xExt(), 2.0, 1e-3);
    EXPECT_NEAR(task.fExt(), 0.0, 1e-3);
}

// 2. Перевірка неправильного інтервалу
TEST(OptimizationTaskTest, ThrowsOnInvalidInterval) {
    EXPECT_THROW(OptimizationTask(5.0, 2.0, 1e-3), std::invalid_argument);
}

// 3. Перевірка некоректної точності
TEST(OptimizationTaskTest, ThrowsOnInvalidTolerance) {
    EXPECT_THROW(OptimizationTask(0.0, 2.0, -0.01), InvalidToleranceException);
}

// 4. Перевірка функції із NaN/Inf
TEST(OptimizationTaskTest, ThrowsOnNonFiniteFunction) {
    OptimizationTask task(0.0, 2.0, 1e-3);
    EXPECT_THROW(task.solveMinimum(testNonFiniteFunc), NonFiniteValueException);
}

// 5. Сильна гарантія (Strong Guarantee)
TEST(OptimizationTaskTest, UpdateAndSolvePreservesStateOnException) {
    OptimizationTask task(0.0, 4.0, 1e-4);
    task.solveMinimum(testSquareFunc);

    const double oldA = task.a();
    const double oldXExt = task.xExt();

    EXPECT_THROW(task.updateAndSolve(3.0, 1.0, 1e-4, 1000, testSquareFunc), std::invalid_argument);

    // Стан не змінився після помилки!
    EXPECT_DOUBLE_EQ(task.a(), oldA);
    EXPECT_DOUBLE_EQ(task.xExt(), oldXExt);
}

// 6. НЕГАТИВНИЙ САМОКОНТРОЛЬ: Тест свідомо викликає Unsafe-метод
TEST(OptimizationTaskTest, DISABLED_UnsafeUpdateViolatesStrongGuarantee) {
    OptimizationTask task(0.0, 4.0, 1e-4);
    task.solveMinimum(testSquareFunc);

    const double oldA = task.a();

    EXPECT_THROW(
        task.updateAndSolveUnsafe(3.0, 1.0, 1e-4, 1000, testSquareFunc),
        std::invalid_argument
    );

    EXPECT_DOUBLE_EQ(task.a(), oldA);
}