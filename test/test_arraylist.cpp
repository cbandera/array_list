#include <array_list/arraylist.hpp>
#include <cmath>
#include "gtest/gtest.h"

// Set up fixtures
class EmptyFixture : public ::testing::Test {
public:
    EmptyFixture(){};
    static constexpr size_t capacity = 5;
    cdt::ArrayList<int, capacity> l;
};
constexpr size_t EmptyFixture::capacity;

class FullFixture : public ::testing::Test {
public:
    FullFixture() {
        for (size_t i = 0; i < capacity; i++) {
            l.push_back(i);
        }
    };
    static constexpr size_t capacity = 5;
    cdt::ArrayList<int, capacity> l;
};
constexpr size_t FullFixture::capacity;

/* ------------------------------------------------------------- */
TEST_F(EmptyFixture, InsertFront) {
    l.push_front(15);
    ASSERT_EQ(15, l.front());
}

TEST_F(EmptyFixture, InsertBack) {
    l.push_back(1);
    ASSERT_EQ(1, l.front());
    l.push_back(2);
    ASSERT_EQ(1, l.front());
    ASSERT_EQ(2, ++l.front());
}

TEST_F(FullFixture, Overfill) {
    ASSERT_EQ(l.max_size(), l.size());
    ASSERT_THROW(l.push_front(0), std::length_error);
}

/* ------------------------------------------------------------- */
TEST_F(EmptyFixture, EmptySize) {
    ASSERT_EQ(0, l.size());
    ASSERT_EQ(true, l.empty());
}

TEST_F(EmptyFixture, MediumSize) {
    ASSERT_EQ(0, l.size());
    l.push_front(3);
    ASSERT_EQ(1, l.size());
    ASSERT_EQ(false, l.empty());
    l.push_front(3);
    ASSERT_EQ(2, l.size());
}

TEST_F(FullFixture, MaxSize) {
    ASSERT_EQ(capacity, l.max_size());
    cdt::ArrayList<int, 15> l2;
    ASSERT_EQ(15, l2.max_size());
}

// TEST_F(FullArraylist, ActualSize) {
//     ASSERT_EQ((capacity + 1) * (                                          // We currently store _N + 1 objects
//                                    sizeof(decltype(l)::value_type) +      // Datatype size
//                                    2 * sizeof(decltype(l)::position_type) // index pointers left/right
//                                    ) +
//                   ceil(capacity / 8.0), // Bitset index
//               sizeof(l));               // Actual size
// }

/* ------------------------------------------------------------- */
TEST_F(FullFixture, SubscriptAccess) {
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(i, l[i]);
    }
    ASSERT_THROW(l[capacity], std::out_of_range);
}

TEST_F(FullFixture, FrontAccess) {
    ASSERT_EQ(0, l.front());
}

TEST_F(FullFixture, BackAccess) {
    ASSERT_EQ(capacity - 1, l.back());
}

TEST_F(FullFixture, IteratorAccess) {
    ASSERT_EQ(0, *l.begin());
}

TEST_F(FullFixture, IteratorLoopAccess) {
    int i = 0;
    for (auto it = l.begin(); it != l.end(); it++) {
        ASSERT_EQ(i, *it);
        i++;
    }
}

TEST_F(FullFixture, IteratorOutOfRangeAccess) {
    ASSERT_THROW(*l.end(), std::out_of_range);
}

/* ------------------------------------------------------------- */
TEST_F(FullFixture, EraseFront) {
    l.erase(l.begin());
    ASSERT_EQ(capacity - 1, l.size());
    ASSERT_EQ(1, l.front());
}

TEST_F(FullFixture, EraseMiddle) {
    l.erase(++l.begin());
    ASSERT_EQ(capacity - 1, l.size());
    ASSERT_EQ(0, l.front());
    ASSERT_EQ(2, *(++l.begin()));
}

TEST_F(FullFixture, EraseEnd) {
    ASSERT_THROW(l.erase(l.end()), std::out_of_range);
}

TEST_F(FullFixture, Clear) {
    l.clear();
    ASSERT_EQ(0, l.size());
    ASSERT_EQ(true, l.empty());
}

/* ------------------------------------------------------------- */
TEST_F(FullFixture, PopFront) {
    for (size_t i = 0; i < capacity; i++) {
        ASSERT_EQ(i, l.front());
        l.pop_front();
        ASSERT_EQ(capacity - i - 1, l.size());
    }
    ASSERT_EQ(0, l.size());
}

TEST_F(FullFixture, PopBack) {
    for (int i = capacity - 1; i >= 0; i--) {
        ASSERT_EQ(i, l.back());
        l.pop_back();
        ASSERT_EQ(i, l.size());
    }
    ASSERT_EQ(0, l.size());
}

/* ------------------------------------------------------------- */
TEST_F(FullFixture, IteratorIncrementPrefix) {
    auto it = l.begin();
    for (size_t i = 0; i < l.size() - 1; i++) {
        ASSERT_EQ(i + 1, *(++it));
    }
}

TEST_F(FullFixture, IteratorIncrementPostfix) {
    auto it = l.begin();
    for (size_t i = 0; i < l.size(); i++) {
        ASSERT_EQ(i, *(it++));
    }
    ASSERT_EQ(it, l.end());
}

TEST_F(FullFixture, IteratorDecrementPrefix) {
    auto it = l.end();
    for (size_t i = l.size(); i > 0; i--) {
        ASSERT_EQ(i - 1, *(--it));
    }
    ASSERT_EQ(it, l.begin());
}

TEST_F(FullFixture, IteratorDecrementPostfix) {
    auto it = l.end();
    it--;
    for (size_t i = l.size(); i > 0; i--) {
        ASSERT_EQ(i - 1, *(it--));
    }
}