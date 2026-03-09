#include <gtest/gtest.h>
#include <Vector.h>
#include <algorithm>
#include <string>

TEST(VectorTest, DefaultConstructorIsEmpty) {
    my_std::Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorTest, PushBackSingleElement) {
    my_std::Vector<int> v;
    v.push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v[0], 42);
}

TEST(VectorTest, PushBackMultipleElements) {
    my_std::Vector<int> v;
    for (int i = 0; i < 25; ++i) {
        v.push_back(i);
    }
    EXPECT_EQ(v.size(), 25);
    for (int i = 0; i < 25; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

TEST(VectorTest, PushBackWithStrings) {
    my_std::Vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}

TEST(VectorTest, PopBackDecreasesSize) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.pop_back();
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, PopBackOnEmptyDoesNothing) {
    my_std::Vector<int> v;
    v.pop_back();
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, ClearResetsSize) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_GT(v.capacity(), 0); // capacity is retained
}

TEST(VectorTest, ReserveIncreasesCapacity) {
    my_std::Vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, ReserveSmallerDoesNothing) {
    my_std::Vector<int> v;
    v.reserve(100);
    auto cap = v.capacity();
    v.reserve(50);
    EXPECT_EQ(v.capacity(), cap);
}

TEST(VectorTest, ReservePreservesElements) {
    my_std::Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.reserve(100);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
}

TEST(VectorTest, BracketOperatorReadWrite) {
    my_std::Vector<int> v;
    v.push_back(5);
    v[0] = 99;
    EXPECT_EQ(v[0], 99);
}

TEST(VectorTest, ConstBracketOperator) {
    my_std::Vector<int> v;
    v.push_back(7);
    const auto& cv = v;
    EXPECT_EQ(cv[0], 7);
}

TEST(VectorTest, AtReturnsCorrectElement) {
    my_std::Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(1), 20);
}

TEST(VectorTest, AtThrowsOnOutOfRange) {
    my_std::Vector<int> v;
    v.push_back(1);
    EXPECT_THROW(v.at(1), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorTest, AtThrowsOnEmpty) {
    my_std::Vector<int> v;
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(VectorTest, ConstAtThrows) {
    my_std::Vector<int> v;
    v.push_back(1);
    const auto& cv = v;
    EXPECT_EQ(cv.at(0), 1);
    EXPECT_THROW(cv.at(1), std::out_of_range);
}

TEST(VectorTest, CopyConstructor) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    my_std::Vector<int> copy(v);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);

    // Ensure deep copy
    v[0] = 99;
    EXPECT_EQ(copy[0], 1);
}

TEST(VectorTest, CopyConstructorEmpty) {
    my_std::Vector<int> v;
    my_std::Vector<int> copy(v);
    EXPECT_TRUE(copy.empty());
}

TEST(VectorTest, CopyAssignment) {
    my_std::Vector<int> v;
    v.push_back(10);
    v.push_back(20);

    my_std::Vector<int> other;
    other.push_back(99);
    other = v;

    EXPECT_EQ(other.size(), 2);
    EXPECT_EQ(other[0], 10);
    EXPECT_EQ(other[1], 20);

    v[0] = 0;
    EXPECT_EQ(other[0], 10); // deep copy
}

TEST(VectorTest, CopyAssignmentSelf) {
    my_std::Vector<int> v;
    v.push_back(1);
    v = v;
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, MoveConstructor) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);

    my_std::Vector<int> moved(std::move(v));
    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, MoveAssignment) {
    my_std::Vector<int> v;
    v.push_back(5);

    my_std::Vector<int> other;
    other = std::move(v);

    EXPECT_EQ(other.size(), 1);
    EXPECT_EQ(other[0], 5);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, CapacityGrowsOnPushBack) {
    my_std::Vector<int> v;
    v.push_back(1);
    EXPECT_GE(v.capacity(), 1);

    size_t cap = v.capacity();
    for (size_t i = 1; i <= cap; ++i) {
        v.push_back(static_cast<int>(i));
    }
    EXPECT_GT(v.capacity(), cap); // should have grown
}

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
};

TEST(VectorTest, EmplaceBackConstructsInPlace) {
    my_std::Vector<Point> v;
    v.emplace_back(3, 4);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0].x, 3);
    EXPECT_EQ(v[0].y, 4);
}

TEST(VectorTest, EmplaceBackMultiple) {
    my_std::Vector<Point> v;
    v.emplace_back(1, 2);
    v.emplace_back(3, 4);
    v.emplace_back(5, 6);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[2].x, 5);
    EXPECT_EQ(v[2].y, 6);
}

TEST(VectorTest, EmplaceBackWithString) {
    my_std::Vector<std::string> v;
    v.emplace_back(5, 'a'); // string(5, 'a') -> "aaaaa"
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "aaaaa");
}

TEST(VectorTest, EmplaceBackGrowsPastCapacity) {
    my_std::Vector<Point> v;
    for (int i = 0; i < 25; ++i) {
        v.emplace_back(i, i * 10);
    }
    EXPECT_EQ(v.size(), 25);
    EXPECT_EQ(v[24].x, 24);
    EXPECT_EQ(v[24].y, 240);
}

TEST(VectorTest, EmplaceBackSingleArg) {
    my_std::Vector<int> v;
    v.emplace_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(VectorTest, BeginEnd) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(VectorTest, RangeBasedFor) {
    my_std::Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    int sum = 0;
    for (auto& x : v) {
        sum += x;
    }
    EXPECT_EQ(sum, 60);
}

TEST(VectorTest, ConstIterators) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    const auto& cv = v;

    int sum = 0;
    for (auto it = cv.begin(); it != cv.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 3);
}

TEST(VectorTest, CBeginCEnd) {
    my_std::Vector<int> v;
    v.push_back(5);
    EXPECT_EQ(*v.cbegin(), 5);
    EXPECT_EQ(v.cend() - v.cbegin(), 1);
}

TEST(VectorTest, EmptyIterators) {
    my_std::Vector<int> v;
    EXPECT_EQ(v.begin(), v.end());
}

TEST(VectorTest, StdAlgorithmSort) {
    my_std::Vector<int> v;
    v.push_back(3);
    v.push_back(1);
    v.push_back(2);
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, StdAlgorithmFind) {
    my_std::Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    auto it = std::find(v.begin(), v.end(), 20);
    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 20);

    auto miss = std::find(v.begin(), v.end(), 99);
    EXPECT_EQ(miss, v.end());
}

TEST(VectorTest, Front) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    EXPECT_EQ(v.front(), 1);
    v.front() = 99;
    EXPECT_EQ(v[0], 99);
}

TEST(VectorTest, Back) {
    my_std::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    EXPECT_EQ(v.back(), 2);
    v.back() = 99;
    EXPECT_EQ(v[1], 99);
}

TEST(VectorTest, ConstFrontBack) {
    my_std::Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    const auto& cv = v;
    EXPECT_EQ(cv.front(), 10);
    EXPECT_EQ(cv.back(), 20);
}

TEST(VectorTest, PushBackMove) {
    my_std::Vector<std::string> v;
    std::string s = "hello";
    v.push_back(std::move(s));
    EXPECT_EQ(v[0], "hello");
    EXPECT_TRUE(s.empty()); // moved-from string should be empty
}

TEST(VectorTest, InitializerListConstructor) {
    my_std::Vector<int> v = {1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InitializerListEmpty) {
    my_std::Vector<int> v = {};
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, InitializerListStrings) {
    my_std::Vector<std::string> v = {"hello", "world"};
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}

TEST(VectorTest, InitializerListThenPushBack) {
    my_std::Vector<int> v = {1, 2};
    v.push_back(3);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[2], 3);
}
