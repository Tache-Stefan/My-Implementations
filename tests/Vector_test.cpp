#include <gtest/gtest.h>
#include <Vector.h>
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
