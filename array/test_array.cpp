#include "array.hpp"

#include <cassert>

void test_element_access();
void test_fill();
void test_swap();

int main() {
    test_element_access();
    test_fill();
    test_swap();
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
    lib::Array<std::string, 3> a;
    a[0] = "str1";
    a[1] = "str2";
    a[2] = "str3";

    lib::Array<std::string, 3> b;
    b[0] = "ABCD";
    b[1] = "str4";
    b[2] = "str5";

    a.swap(b);

    assert(a[0] == "ABCD" && b[0] == "str1");
    assert(a[1] == "str4" && b[1] == "str2");
    assert(a[2] == "str5" && b[2] == "str3");
}
