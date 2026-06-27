#include "vector.hpp"

#include <cassert>
#include <print>
#include <string>

void test_from_default_constructor();
void test_copy_constructor();
void test_move_constructor();
void test_init_constructor();
void test_reserve();

int main() {
    test_from_default_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_init_constructor();
    test_reserve();

    std::println("All tests passed!");
}

void test_from_default_constructor() {
    lib::Vector<std::string> v1;
    assert(v1.capacity() == 0);
    assert(v1.size() == 0);

    v1.push_back("String");
    assert(v1.size() == 1);

    v1.push_back("");
    assert(v1.size() == 2);

    assert(v1[0] == "String");
    assert(v1[1] == "");

    v1.pop_back();
    assert(v1.size() == 1);

    v1.pop_back();
    assert(v1.size() == 0);

    v1.pop_back();
    assert(v1.size() == 0);

    v1.push_back("String");
    assert(v1.size() == 1);
}

void test_copy_constructor() {
    lib::Vector<std::string> vec(5);
    assert((int)vec.size() == 0);
    vec.emplace_back("1");
    vec.emplace_back("2");
    vec.emplace_back("3");
    vec.emplace_back("4");
    vec.emplace_back("5");
    assert((int)vec.size() == 5);

    auto vec_copy = vec;
    assert((int)vec.size() == (int)vec_copy.size());
    assert(vec.data() != vec_copy.data());

    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == vec_copy[i]);
    }
}

void test_move_constructor() {
    lib::Vector<std::string> vec(5);
    assert((int)vec.capacity() == 5);
    assert((int)vec.size() == 0);
    vec.push_back("0");
    vec.push_back("1");
    vec.push_back("2");
    vec.push_back("3");
    vec.push_back("4");
    assert((int)vec.capacity() == 5);
    assert((int)vec.size() == 5);

    auto vec_buffer_ptr = vec.data();
    auto vec_size = vec.size();
    auto vec_capacity = vec.capacity();

    auto vec_moved = std::move(vec);
    assert(vec.data() == nullptr);

    assert(vec_moved.data() == vec_buffer_ptr);
    assert(vec_moved.size() == vec_size);
    assert(vec_moved.capacity() == vec_capacity);

    for (size_t i = 0; i < vec_moved.size(); i++) {
        assert(vec_moved[i] == std::to_string(i));
    }
}

void test_init_constructor() {
    lib::Vector<std::string> vec(5, "init_string");
    assert(vec.size() == 5);
    assert(vec.capacity() == 5);

    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == "init_string");
    }
}

void test_reserve() {
    lib::Vector<std::string> vec(3, "init");
    auto* buffer = vec.data();

    vec.reserve(100);
    // New buffer was allocated
    assert(buffer != vec.data());

    assert(vec.capacity() == 100);
    for (size_t i = 0; i < vec.size(); i++) {
        assert(vec[i] == "init");
    }

    buffer = vec.data();
    for (size_t i = 0; i < (vec.capacity() - vec.size()); i++) {
        vec.push_back(std::to_string(i));
    }
    // assert no new buffer was made
    assert(vec.data() == buffer);
}
