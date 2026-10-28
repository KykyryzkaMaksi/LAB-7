#include <iostream>
#include <stdexcept>

void throwPrimitiveInt() {
    throw 42;
}

void demonstrateSlicing() {
    try {
        throw std::invalid_argument("Original Argument Exception");
    }
    catch (const std::exception& ex) {
        throw ex;
    }
}

int main() {
    std::cout << "=== Antipattern Demo ===\n\n";

    try {
        throwPrimitiveInt();
    }
    catch (const std::exception&) {
        std::cout << "std::exception handler missed primitive int!\n";
    }
    catch (int err) {
        std::cout << "Caught primitive int: " << err << '\n';
    }

    std::cout << "\n--- Object Slicing Demo ---\n";
    try {
        demonstrateSlicing();
    }
    catch (const std::invalid_argument&) {
        std::cout << "Caught invalid_argument\n";
    }
    catch (const std::exception&) {
        std::cout << "Caught base std::exception (Object Slicing occurred!)\n";
    }

    return 0;
}