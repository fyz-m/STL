#include "shared_ptr.hpp"

#include <cassert>

void test();

void test_copy_constructor();

void test_move_constructor();

void test_copy_assignment();

void test_move_assignment();

struct TestDummy;

int main() {
    test();
}

void test() {
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();
}

void test_copy_constructor() {
    bool destroyed{false};
    {
        auto test_dummy1 = lib::make_shared<TestDummy>(destroyed);
        {
            auto test_dummy2{test_dummy1};
            assert(test_dummy1.useCount() == test_dummy2.useCount() && test_dummy1.useCount() == 2);
        }
        assert(destroyed == false);
        assert(test_dummy1.isUnique());
    }
    assert(destroyed);
}

void test_move_constructor() {
    {
        bool destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(destroyed);
            auto test_dummy2{std::move(test_dummy1)};
            assert(test_dummy2.useCount() == 1);
        }
        assert(destroyed);
    }

    {
        bool destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(destroyed);
            auto test_dummy2{std::move(test_dummy1)};
            auto test_dummy3{std::move(test_dummy2)};
            assert(test_dummy3.useCount() == 1);
        }
        assert(destroyed);
    }
}

void test_copy_assignment() {
    {
        bool destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(destroyed);
            {
                auto test_dummy2 = test_dummy1;
                assert(test_dummy2.useCount() == 2);
                assert(test_dummy2.get() == test_dummy1.get());
            }
            assert(test_dummy1.isUnique());
        }
        assert(destroyed);
    }

    // Test chained copy assignments
    {
        bool destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(destroyed);
            {
                lib::shared_ptr<TestDummy> test_dummy2;
                auto test_dummy3 = test_dummy2 = test_dummy1;
                assert(test_dummy2.useCount() == 3);
                assert(test_dummy3.get() == test_dummy2.get() &&
                       test_dummy2.get() == test_dummy1.get());
            }
            assert(test_dummy1.isUnique());
        }
        assert(destroyed);
    }

    // Test copying into existed shared releases its resources
    {
        bool d1_destroyed{false};
        bool d2_destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(d1_destroyed);
            {
                auto test_dummy2 = lib::make_shared<TestDummy>(d2_destroyed);
                test_dummy1 = test_dummy2;
                assert(d1_destroyed);
                assert(test_dummy2.useCount() == 2);
            }
        }
        assert(d2_destroyed);
    }
}

void test_move_assignment() {
    {
        bool destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(destroyed);
            auto test_dummy1_ptr = test_dummy1.get();
            {
                auto test_dummy2 = std::move(test_dummy1);
                assert(test_dummy2.useCount() == 1);
                assert(test_dummy2.get() == test_dummy1_ptr);
            }
            assert(destroyed);
        }
    }

    // Test chained move assignments
    {
        bool destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(destroyed);
            {
                lib::shared_ptr<TestDummy> test_dummy2;
                lib::shared_ptr<TestDummy> test_dummy3;
                test_dummy3 = std::move(test_dummy2 = std::move(test_dummy1));
                assert(test_dummy3.useCount() == 1);
            }
            assert(destroyed);
        }
    }

    // Test moving into existed shared_ptr releases its resources
    {
        bool d1_destroyed{false};
        bool d2_destroyed{false};
        {
            auto test_dummy1 = lib::make_shared<TestDummy>(d1_destroyed);
            {
                auto test_dummy2 = lib::make_shared<TestDummy>(d2_destroyed);
                test_dummy1 = std::move(test_dummy2);
                assert(d1_destroyed);
                assert(test_dummy1.useCount() == 1);
            }
            assert(d2_destroyed == false);
        }
        assert(d2_destroyed);
    }
}

struct TestDummy {
    bool moved{false};
    bool copied{false};
    bool& destroyed;

    TestDummy(bool& destroyed) : copied(false), moved(false), destroyed(destroyed) {}

    TestDummy(const TestDummy&, bool& destroyed) : copied{true}, destroyed{destroyed} {}
    TestDummy(const TestDummy&&, bool& destroyed) : moved{true}, destroyed{destroyed} {}

    TestDummy& operator=(const TestDummy& t) {
        copied = true;
        destroyed = t.destroyed;
        return *this;
    };

    TestDummy& operator=(TestDummy&& t) {
        moved = true;
        destroyed = t.destroyed;
        return *this;
    };

    ~TestDummy() { destroyed = true; }
};