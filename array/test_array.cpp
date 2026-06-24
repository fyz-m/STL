#include "array.hpp"

#include <cassert>
#include <print>

void test_default_constructor();
void test_copy_constructor();
void test_move_constructor();

int main() {
    test_default_constructor();
    test_move_constructor();
    test_copy_constructor();
}

void test_default_constructor() {
    lib::Array<int, 5> a;
    for (int i = 0; i < 5; ++i) {
        a[i] = 5 - i;
    }
    for (int i = 0; i < 5; ++i) {
        assert(a[i] == 5 - i);
    }
}

void test_copy_constructor() {
    lib::Array<float, 10> a;
    for (int i = 0; i < 10; ++i) {
        a[i] = 10 - i;
    }

    auto b = a;
    for (int i = 0; i < 10; ++i) {
        assert(a[i] == b[i]);
    }
}

void test_move_constructor() {}
