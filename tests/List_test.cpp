#include <gtest/gtest.h>
#include <List.h>
#include <string>
#include <vector>

TEST(ListTest, DefaultConstructorIsEmpty) {
    my_std::List<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST(ListTest, PushBackSingleElement) {
    my_std::List<int> l;
    l.push_back(42);
    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(l.front(), 42);
    EXPECT_EQ(l.back(), 42);
}

TEST(ListTest, PushFrontSingleElement) {
    my_std::List<int> l;
    l.push_front(42);
    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(l.front(), 42);
    EXPECT_EQ(l.back(), 42);
}

TEST(ListTest, PushBackAndPushFrontOrder) {
    my_std::List<int> l;
    l.push_back(2);
    l.push_front(1);
    l.push_back(3);

    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, PopBackRemovesLast) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    l.pop_back();
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.back(), 2);
    EXPECT_EQ(l.front(), 1);
}

TEST(ListTest, PopFrontRemovesFirst) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    l.pop_front();
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), 2);
    EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, PopOnEmptyDoesNothing) {
    my_std::List<int> l;
    l.pop_back();
    l.pop_front();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST(ListTest, ClearResetsContainer) {
    my_std::List<int> l;
    l.push_back(10);
    l.push_back(20);
    l.clear();

    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST(ListTest, ResizeGrowDefaultConstructs) {
    my_std::List<int> l;
    l.push_back(5);
    l.resize(4);

    EXPECT_EQ(l.size(), 4);
    EXPECT_EQ(l.front(), 5);
    EXPECT_EQ(l.back(), 0);
}

TEST(ListTest, ResizeGrowWithValue) {
    my_std::List<int> l;
    l.push_back(1);
    l.resize(4, 7);

    EXPECT_EQ(l.size(), 4);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 7);
}

TEST(ListTest, ResizeShrink) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);

    l.resize(2);
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 2);
}

TEST(ListTest, CopyConstructorDeepCopy) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);

    my_std::List<int> copy(l);
    l.front() = 99;

    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 2);
}

TEST(ListTest, CopyAssignmentDeepCopy) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);

    my_std::List<int> other;
    other.push_back(100);
    other = l;

    l.back() = 77;

    EXPECT_EQ(other.size(), 2);
    EXPECT_EQ(other.front(), 1);
    EXPECT_EQ(other.back(), 2);
}

TEST(ListTest, MoveConstructorTransfersOwnership) {
    my_std::List<int> l;
    l.push_back(10);
    l.push_back(20);

    my_std::List<int> moved(std::move(l));

    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 20);
    EXPECT_TRUE(l.empty());
}

TEST(ListTest, MoveAssignmentTransfersOwnership) {
    my_std::List<int> l;
    l.push_back(10);
    l.push_back(20);

    my_std::List<int> other;
    other.push_back(1);
    other = std::move(l);

    EXPECT_EQ(other.size(), 2);
    EXPECT_EQ(other.front(), 10);
    EXPECT_EQ(other.back(), 20);
    EXPECT_TRUE(l.empty());
}

TEST(ListTest, IteratorForwardTraversal) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    int sum = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 6);
}

TEST(ListTest, IteratorModifiesElements) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    for (auto it = l.begin(); it != l.end(); ++it) {
        *it *= 2;
    }

    EXPECT_EQ(l.front(), 2);
    EXPECT_EQ(l.back(), 6);
}

TEST(ListTest, ConstIteratorTraversal) {
    my_std::List<int> l;
    l.push_back(4);
    l.push_back(5);

    const auto& cl = l;
    int sum = 0;
    for (auto it = cl.cbegin(); it != cl.cend(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 9);
}

TEST(ListTest, EmptyIteratorsMatch) {
    my_std::List<int> l;
    EXPECT_EQ(l.begin(), l.end());
    EXPECT_EQ(l.cbegin(), l.cend());
}

TEST(ListTest, DecrementEndGivesLastElement) {
    my_std::List<int> l;
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);

    auto it = l.end();
    --it;
    EXPECT_EQ(*it, 30);
}

TEST(ListTest, ReverseIteratorTraversal) {
    my_std::List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    std::vector<int> out;
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        out.push_back(*it);
    }

    EXPECT_EQ((std::vector<int>{3, 2, 1}), out);
}

TEST(ListTest, WorksWithStrings) {
    my_std::List<std::string> l;
    l.push_back("hello");
    l.push_front("world");

    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), "world");
    EXPECT_EQ(l.back(), "hello");
}
