## Інваріанти класу OptimizationTask
1. `a_ < b_` — інтервал є строго впорядкованим.
2. `eps_ > 0 && isfinite(eps_)` — точність є додатним скінченним числом.
3. `maxIter_ > 0` — кількість ітерацій більша за нуль.

## Сильна гарантія безпеки (Strong Guarantee)
Забезпечується методом `OptimizationTask::updateAndSolve()` за принципом **prepare-then-commit**:
спочатку у тимчасовому об'єкті створюється новий стан і проводяться обчислення. Лише після успішного виконання без винятків значення копіюються у поточний об'єкт.

## Очікувана послідовність логів стека (ScopeLogger)
```text
[ENTER] level1
[ENTER] level2
[ENTER] level3
[EXIT ] level3
[LOG level2] Intercepted exception: Invalid tolerance or maxIter parameter: -0.010000
[EXIT ] level2
[EXIT ] level1
[MAIN] Caught InvalidToleranceException! Value: -0.01