#include <gtest/gtest.h>
#include <Array.h>
#include <string>

TEST(ArrayTest, SizeReturnsN) {
    my_std::Array<int, 5> arr;
    EXPECT_EQ(arr.size(), 5);
}

TEST(ArrayTest, MaxSizeReturnsN) {
    my_std::Array<int, 5> arr;
    EXPECT_EQ(arr.max_size(), 5);
}

TEST(ArrayTest, EmptyReturnsFalseWhenNonZero) {
    my_std::Array<int, 3> arr;
    EXPECT_FALSE(arr.empty());
}

TEST(ArrayTest, EmptyReturnsTrueWhenZero) {
    my_std::Array<int, 0> arr;
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, OperatorBracketReadWrite) {
    my_std::Array<int, 3> arr;
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, AtValidIndex) {
    my_std::Array<int, 3> arr;
    arr[0] = 42;
    EXPECT_EQ(arr.at(0), 42);
}

TEST(ArrayTest, AtThrowsOnOutOfRange) {
    my_std::Array<int, 3> arr;
    EXPECT_THROW(arr.at(3), std::out_of_range);
    EXPECT_THROW(arr.at(100), std::out_of_range);
}

TEST(ArrayTest, AtThrowsOnEmptyArray) {
    my_std::Array<int, 0> arr;
    EXPECT_THROW(arr.at(0), std::out_of_range);
}

TEST(ArrayTest, ConstAt) {
    my_std::Array<int, 3> arr;
    arr[1] = 99;
    const auto& carr = arr;
    EXPECT_EQ(carr.at(1), 99);
    EXPECT_THROW(carr.at(3), std::out_of_range);
}

TEST(ArrayTest, FrontAndBack) {
    my_std::Array<int, 4> arr;
    arr[0] = 1;
    arr[3] = 4;
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 4);
}

TEST(ArrayTest, ConstFrontAndBack) {
    my_std::Array<int, 3> arr;
    arr[0] = 10;
    arr[2] = 30;
    const auto& carr = arr;
    EXPECT_EQ(carr.front(), 10);
    EXPECT_EQ(carr.back(), 30);
}

TEST(ArrayTest, DataReturnsPointerToFirstElement) {
    my_std::Array<int, 3> arr;
    arr[0] = 5;
    EXPECT_EQ(*arr.data(), 5);
    EXPECT_EQ(arr.data(), &arr[0]);
}

TEST(ArrayTest, BeginEndIterators) {
    my_std::Array<int, 3> arr;
    arr[0] = 1; arr[1] = 2; arr[2] = 3;

    int sum = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(ArrayTest, RangeBasedFor) {
    my_std::Array<int, 4> arr;
    arr.fill(10);

    int sum = 0;
    for (int val : arr) {
        sum += val;
    }
    EXPECT_EQ(sum, 40);
}

TEST(ArrayTest, ConstIterators) {
    my_std::Array<int, 3> arr;
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    const auto& carr = arr;

    int sum = 0;
    for (auto it = carr.cbegin(); it != carr.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(ArrayTest, IteratorDistance) {
    my_std::Array<int, 5> arr;
    EXPECT_EQ(arr.end() - arr.begin(), 5);
}

TEST(ArrayTest, EmptyArrayIterators) {
    my_std::Array<int, 0> arr;
    EXPECT_EQ(arr.begin(), arr.end());
    EXPECT_EQ(arr.cbegin(), arr.cend());
}

TEST(ArrayTest, FillSetsAllElements) {
    my_std::Array<int, 5> arr;
    arr.fill(42);
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 42);
    }
}

TEST(ArrayTest, FillOnEmptyArray) {
    my_std::Array<int, 0> arr;
    arr.fill(99); // should not crash
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, SwapExchangesContents) {
    my_std::Array<int, 3> a, b;
    a.fill(1);
    b.fill(2);

    a.swap(b);

    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(a[i], 2);
        EXPECT_EQ(b[i], 1);
    }
}

TEST(ArrayTest, WorksWithStrings) {
    my_std::Array<std::string, 2> arr;
    arr[0] = "hello";
    arr[1] = "world";
    EXPECT_EQ(arr.at(0), "hello");
    EXPECT_EQ(arr.at(1), "world");
    EXPECT_EQ(arr.size(), 2);
}

TEST(ArrayTest, WorksWithDoubles) {
    my_std::Array<double, 3> arr;
    arr.fill(3.14);
    EXPECT_DOUBLE_EQ(arr[0], 3.14);
    EXPECT_DOUBLE_EQ(arr.back(), 3.14);
}
