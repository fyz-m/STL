#include "array.hpp"

#include <cassert>
#include <print>
#include <utility>

void test_default_constructor();
void test_copy_constructor();
void test_move_constructor();
void test_copy_assignment();
void test_move_assignment();
void test_element_access();
void test_fill();
void test_swap();

int main() {
    test_default_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();
    test_element_access();
    test_fill();
    test_swap();
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

void test_move_constructor() {
    lib::Array<int, 10> a;
    for (int i = 0; i < 10; ++i) {
        a[i] = 10 - i;
    }
    auto b = std::move(a);
    assert(a.data() == nullptr);
    for (int i = 0; i < 10; ++i) {
        assert(b[i] = 10 - i);
    }
}

void test_copy_assignment() {
    lib::Array<float, 10> a;
    for (int i = 0; i < 10; ++i) {
        a[i] = 10 - i;
    }

    lib::Array<float, 10> b;
    b = a;
    for (int i = 0; i < 10; ++i) {
        assert(a[i] == b[i]);
    }
}

void test_move_assignment() {
    lib::Array<float, 10> a;
    for (int i = 0; i < 10; ++i) {
        a[i] = 10 - i;
    }

    lib::Array<float, 10> b;
    b = std::move(a);
    assert(a.data() == nullptr);
    for (int i = 0; i < 10; ++i) {
        assert(10 - i == b[i]);
    }
}

void test_element_access() {
    lib::Array<int, 10> a;
    a[0] = 10;
    a[2] = 20;
    a[3] = 30;
    a[4] = 40;
    a[5] = 50;
    a[6] = 60;
    a[7] = 70;
    a[8] = 8000;
    a[9] = 1234;

    assert(a.front() == 10);
    assert(a.back() == 1234);
    assert(a.at(3) == 30);
    assert(a.at(2) == 20);
    assert(a.at(8) == 8000);
    assert(a.at(9) == 1234);
    assert(a.at(0) == 10);

    int error_count{0};
    while (error_count < 3) {
        try {
            a.at(-1);
            a.at(10);
            a.at(100);
            break;
        } catch (const std::out_of_range& e) {
            error_count = error_count + 1;
            continue;
        }
    }
    assert(error_count == 3);

    assert(a.empty() == false);
    lib::Array<int, 0> b;
    assert(b.empty());
}

void test_fill() {
    lib::Array<int, 100> a;

    a.fill(3);
    for (int i = 0; i < 100; ++i) {
        assert(a[i] == 3);
    }
}

void test_swap() {
    lib::Array<int, 3> a;
    a[0] = 20;
    a[1] = 25;
    a[2] = 30;

    lib::Array<int, 3> b;
    b[0] = 1;
    b[1] = 2;
    b[2] = 3;

    a.swap(b);

    assert(a[0] == 1 && b[0] == 20);
    assert(a[1] == 2 && b[1] == 25);
    assert(a[2] == 3 && b[2] == 30);
}
