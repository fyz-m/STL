#include "vector.hpp"

#include <cassert>
#include <print>
#include <string>

namespace {

void test_from_default_constructor();
void test_copy_constructor();
void test_move_constructor();
void test_init_constructor();
void test_reserve();
void test_copy_assignment();
void test_move_assignment();
void test_emplace();

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
    // Rvalue initialization value
    lib::Vector<std::string> vec(5, "init_string");
    assert(vec.size() == 5);
    assert(vec.capacity() == 5);

    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == "init_string");
    }

    // Lvalue initialization value
    std::string str = "init_str";
    lib::Vector<std::string> vec2(5, str);
    assert(vec2.size() == 5);
    assert(vec2.capacity() == 5);

    for (size_t i = 0; i < vec2.size(); ++i) {
        assert(vec2[i] == str);
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

void test_copy_assignment() {
    lib::Vector<std::string> vec{5, "init"};
    lib::Vector<std::string> vec2{3, "init2"};
    auto* vec2_old_buffer = vec2.data();

    vec2 = vec;
    assert(vec.size() == vec2.size());
    assert(vec.capacity() == vec2.capacity());
    assert(vec.data() != vec2.data());
    assert(vec.data() != vec2_old_buffer);

    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == vec2[i]);
    }
}

void test_move_assignment() {
    lib::Vector<std::string> vec{5, "init"};
    auto vec_size = vec.size();
    auto vec_capacity = vec.capacity();
    auto* vec_buffer = vec.data();

    lib::Vector<std::string> vec2{3, "init2"};

    vec2 = std::move(vec);
    assert(vec.data() == nullptr);
    assert(vec_buffer == vec2.data());
    assert(vec_size == vec2.size());
    assert(vec_capacity == vec2.capacity());

    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec2[i] == "init");
    }
}

struct test {

    bool* moved{};
    bool* copied{};
    bool* destroyed{};
    bool constructed{false};

    test() = default;

    test(bool* moved, bool* copied)
        : moved{moved}, copied{copied}, constructed(true) {}

    test(bool* destroyed, bool* moved, bool* copied)
        : moved{moved}, copied{copied}, destroyed(destroyed), constructed(true) {}

    test(const test& other) {
        *other.copied = true;
    }

    test& operator=(const test& other) {
        *other.copied = true;
        return *this;
    }

    test& operator=(test&& other) {
        *other.moved = true;
        return *this;
    }

    test(test&& other) noexcept {
        *other.moved = true;
    }

    ~test() {
        if (destroyed)
            *destroyed = true;
    }
};

void test_emplace() {
    lib::Vector<test> vec;
    bool moved = false;
    bool copied = false;

    vec.emplace_back(&moved, &copied);
    assert(vec[0].constructed == true);
    assert(*vec[0].copied == false);
    assert(*vec[0].moved == false);

    vec.emplace_back(&moved, &copied);
    assert(vec[1].constructed == true);
    assert(*vec[1].copied == false);
    assert(*vec[1].moved == false);

    vec.push_back(vec[0]);
    assert(*vec[0].copied == true);

    test t1(&moved, &copied);
    vec.push_back(t1);
    assert(*t1.copied == true);

    test t2(&moved, &copied);
    vec.push_back(std::move(t2));
    assert(*t2.moved == true);

    bool destroyed = false;
    vec.emplace_back(&destroyed, &moved, &copied);
    auto size = vec.size();
    auto capacity = vec.capacity();

    vec.pop_back();
    assert(vec.size() == (size - 1));
    assert(vec.capacity() == capacity);
    assert(destroyed);
}

} // namespace

int main() {
    test_from_default_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_init_constructor();
    test_reserve();
    test_copy_assignment();
    test_move_assignment();
    test_emplace();

    std::println("All tests passed!");
}