#include "vector.hpp"

#include <print>
#include <vector>
int main() {
    lib::Vector<int> a(3, 100);

    std::println("Size: {}", a.size());
    std::println("Capacity: {}", a.capacity());

    a.reserve(100);

    std::println("New Size: {}", a.size());
    std::println("New Capacity: {}", a.capacity());

    for (size_t i = 0; i < a.size(); ++i) {
        std::println("a[{}] = {}", i, a[i]);
    }

    a.push_back(40);
    a.push_back(70);
    std::println("\n\n---------------AFTER PUSHBACK---------------");
    for (size_t i = 0; i < a.size(); ++i) {
        std::println("a[{}] = {}", i, a[i]);
    }
}