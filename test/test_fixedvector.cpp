#include <array_list/fixedvector.hpp>
#include <cmath>
#include "gtest/gtest.h"
#include <algorithm>

// Set up fixtures
class EmptyFixedvector : public ::testing::Test {
public:
    EmptyFixedvector(){};
    static constexpr size_t capacity = 5;
    cdt::FixedVector<int, capacity> l;
};
constexpr size_t EmptyFixedvector::capacity;

class FullFixedvector : public ::testing::Test {
public:
    FullFixedvector() {
        for (size_t i = 0; i < capacity; i++) {
            l.push_back(i);
        }
    };
    static constexpr size_t capacity = 5;
    cdt::FixedVector<int, capacity> l;
};
constexpr size_t FullFixedvector::capacity;

/* ------------------------------------------------------------- */
TEST_F(EmptyFixedvector, InsertBack) {
    l.push_back(1);
    ASSERT_EQ(1, l.front());
    l.push_back(2);
    ASSERT_EQ(1, l.front());
    ASSERT_EQ(2, ++l.front());
}

TEST_F(FullFixedvector, InitialData) {
    ASSERT_EQ(l.capacity(), l.size());
    for (size_t i = 0; i < l.capacity(); i++) {
        ASSERT_EQ(i, l[i]);
    }
}

TEST_F(FullFixedvector, Assertion) {
    ASSERT_THROW(l[l.size()], std::out_of_range);
    ASSERT_THROW(l[l.capacity()], std::out_of_range);
    ASSERT_THROW(l[l.capacity()+1], std::out_of_range);
    ASSERT_THROW(l[-1], std::out_of_range);
}

TEST_F(FullFixedvector, Overfill) {
    ASSERT_EQ(l.capacity(), l.size());
    ASSERT_THROW(l.push_back(0), std::out_of_range);
}

/* ------------------------------------------------------------- */
TEST_F(EmptyFixedvector, EmptySize) {
    ASSERT_EQ(0, l.size());
    ASSERT_EQ(true, l.empty());
}

TEST_F(EmptyFixedvector, MediumSize) {
    ASSERT_EQ(0, l.size());
    l.push_back(3);
    ASSERT_EQ(1, l.size());
    ASSERT_EQ(false, l.empty());
    l.push_back(3);
    ASSERT_EQ(2, l.size());
}

TEST_F(FullFixedvector, MaxSize) {
    ASSERT_EQ(capacity, l.capacity());
    cdt::FixedVector<int, 15> l2;
    ASSERT_EQ(15, l2.capacity());
}

// TEST_F(FullFixedvector, ActualSize) {
//     ASSERT_EQ(capacity * sizeof(decltype(l)::value_type) + // Datatype size
//                   1 * sizeof(decltype(l)::position_type),  // end_index
//               sizeof(l));                                  // Actual size
// }

/* ------------------------------------------------------------- */
TEST_F(FullFixedvector, SubscriptAccess) {
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(i, l[i]);
    }
    ASSERT_THROW(l[capacity], std::out_of_range);
}

TEST_F(FullFixedvector, FrontAccess) {
    ASSERT_EQ(0, l.front());
}

TEST_F(FullFixedvector, BackAccess) {
    ASSERT_EQ(capacity - 1, l.back());
}

TEST_F(FullFixedvector, IteratorAccess) {
    ASSERT_EQ(0, *l.begin());
}

TEST_F(FullFixedvector, IteratorLoopAccess) {
    int i = 0;
    for (auto it = l.begin(); it != l.end(); it++) {
        ASSERT_EQ(i, *it);
        i++;
    }
}

/* ------------------------------------------------------------- */
TEST_F(FullFixedvector, EraseFront) {
    l.erase(l.begin());
    ASSERT_EQ(capacity - 1, l.size());
    ASSERT_EQ(std::find(l.begin(), l.end(), 0), l.end());
}

TEST_F(FullFixedvector, EraseMiddle) {
    l.erase(1);
    ASSERT_EQ(capacity - 1, l.size());
    ASSERT_EQ(std::find(l.begin(), l.end(), 1), l.end());
}

TEST_F(FullFixedvector, EraseEnd) {
    ASSERT_THROW(l.erase(l.end()), std::out_of_range);
}

TEST_F(FullFixedvector, Clear) {
    l.clear();
    ASSERT_EQ(0, l.size());
    ASSERT_EQ(true, l.empty());
}

/* ------------------------------------------------------------- */
TEST_F(FullFixedvector, PopFront) {
    for (size_t i = 0; i < capacity; i++) {
        l.pop_front();
        ASSERT_EQ(capacity - i - 1, l.size());
    }
    ASSERT_EQ(0, l.size());
}

TEST_F(FullFixedvector, PopBack) {
    for (int i = capacity - 1; i >= 0; i--) {
        ASSERT_NE(std::find(l.begin(), l.end(), i), l.end());
        l.pop_back();
        ASSERT_EQ(std::find(l.begin(), l.end(), i), l.end());
    }
    ASSERT_EQ(0, l.size());
}

/* ------------------------------------------------------------- */
TEST_F(FullFixedvector, IteratorIncrementPrefix) {
    auto it = l.begin();
    for (size_t i = 0; i < l.size() - 1; i++) {
        ASSERT_EQ(i + 1, *(++it));
    }
}

TEST_F(FullFixedvector, IteratorIncrementPostfix) {
    auto it = l.begin();
    for (size_t i = 0; i < l.size(); i++) {
        ASSERT_EQ(i, *(it++));
    }
    ASSERT_EQ(it, l.end());
}

TEST_F(FullFixedvector, IteratorDecrementPrefix) {
    auto it = l.end();
    for (size_t i = l.size(); i > 0; i--) {
        ASSERT_EQ(i - 1, *(--it));
    }
    ASSERT_EQ(it, l.begin());
}

TEST_F(FullFixedvector, IteratorDecrementPostfix) {
    auto it = l.end();
    it--;
    for (size_t i = l.size(); i > 0; i--) {
        ASSERT_EQ(i - 1, *(it--));
    }
}

/// TODO
/// - Copy constructor -> size()?
/// Test valid pointers