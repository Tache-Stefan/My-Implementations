#include <gtest/gtest.h>
#include <Unordered_Map.h>
#include <string>
#include <vector>

TEST(UnorderedMapTest, DefaultConstructorIsEmpty) {
    Unordered_Map<int, int> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

TEST(UnorderedMapTest, InsertAndAtReturnsValue) {
    Unordered_Map<std::string, int> m;
    m.insert("one", 1);
    m.insert("two", 2);

    EXPECT_EQ(m.size(), 2u);
    EXPECT_EQ(m.at("one"), 1);
    EXPECT_EQ(m.at("two"), 2);
}

TEST(UnorderedMapTest, InsertOverwritesExistingValue) {
    Unordered_Map<int, std::string> m;
    m.insert(1, "a");
    m.insert(1, "b");

    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.at(1), "b");
}

TEST(UnorderedMapTest, AtThrowsOnMissingKey) {
    Unordered_Map<int, int> m;
    EXPECT_THROW(m.at(5), std::out_of_range);
}

TEST(UnorderedMapTest, CountAndContains) {
    Unordered_Map<int, int> m;
    m.insert(7, 70);

    EXPECT_EQ(m.count(7), 1u);
    EXPECT_TRUE(m.contains(7));
    EXPECT_EQ(m.count(8), 0u);
    EXPECT_FALSE(m.contains(8));
}

TEST(UnorderedMapTest, BracketInsertsDefaultAndReturnsRef) {
    Unordered_Map<std::string, int> m;
    int& v = m["x"];
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(v, 0);

    v = 42;
    EXPECT_EQ(m.at("x"), 42);
}

TEST(UnorderedMapTest, BracketFindsExisting) {
    Unordered_Map<int, int> m;
    m.insert(3, 30);

    m[3] = 99;
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.at(3), 99);
}

TEST(UnorderedMapTest, FindReturnsIteratorOrEnd) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);

    auto it = m.find(2);
    EXPECT_NE(it, m.end());
    EXPECT_EQ(it->key, 2);
    EXPECT_EQ(it->value, 20);

    auto miss = m.find(99);
    EXPECT_EQ(miss, m.end());
}

TEST(UnorderedMapTest, EraseRemovesExistingKey) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);

    EXPECT_EQ(m.erase(1), 1u);
    EXPECT_EQ(m.size(), 1u);
    EXPECT_FALSE(m.contains(1));
    EXPECT_TRUE(m.contains(2));
}

TEST(UnorderedMapTest, EraseMissingReturnsZero) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);

    EXPECT_EQ(m.erase(2), 0u);
    EXPECT_EQ(m.size(), 1u);
}

TEST(UnorderedMapTest, ClearEmptiesMap) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);
    m.clear();

    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_FALSE(m.contains(1));
}

TEST(UnorderedMapTest, CopyConstructorDeepCopy) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);

    Unordered_Map<int, int> copy(m);
    m.insert(3, 30);
    m.insert(1, 99);

    EXPECT_EQ(copy.size(), 2u);
    EXPECT_EQ(copy.at(1), 10);
    EXPECT_EQ(copy.at(2), 20);
    EXPECT_FALSE(copy.contains(3));
}

TEST(UnorderedMapTest, CopyAssignmentDeepCopy) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);

    Unordered_Map<int, int> other;
    other.insert(9, 90);
    other = m;

    m.insert(3, 30);
    m.insert(2, 200);

    EXPECT_EQ(other.size(), 2u);
    EXPECT_EQ(other.at(1), 10);
    EXPECT_EQ(other.at(2), 20);
    EXPECT_FALSE(other.contains(3));
}

TEST(UnorderedMapTest, MoveConstructorTransfersOwnership) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);

    Unordered_Map<int, int> moved(std::move(m));
    EXPECT_EQ(moved.size(), 2u);
    EXPECT_EQ(moved.at(1), 10);
    EXPECT_EQ(moved.at(2), 20);
    EXPECT_TRUE(m.empty());
}

TEST(UnorderedMapTest, MoveAssignmentTransfersOwnership) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);

    Unordered_Map<int, int> other;
    other.insert(9, 90);

    other = std::move(m);

    EXPECT_EQ(other.size(), 2u);
    EXPECT_EQ(other.at(1), 10);
    EXPECT_EQ(other.at(2), 20);
    EXPECT_TRUE(m.empty());
}

TEST(UnorderedMapTest, IteratorTraversesAllElements) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);
    m.insert(3, 30);

    int sum_keys = 0;
    int sum_vals = 0;
    for (auto it = m.begin(); it != m.end(); ++it) {
        sum_keys += it->key;
        sum_vals += it->value;
    }

    EXPECT_EQ(sum_keys, 1 + 2 + 3);
    EXPECT_EQ(sum_vals, 10 + 20 + 30);
}

TEST(UnorderedMapTest, ConstIteratorWorks) {
    Unordered_Map<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);

    const auto& cm = m;
    int sum_vals = 0;
    for (auto it = cm.begin(); it != cm.end(); ++it) {
        sum_vals += it->value;
    }

    EXPECT_EQ(sum_vals, 30);
}

TEST(UnorderedMapTest, RehashPreservesElements) {
    Unordered_Map<int, int> m(4);
    for (int i = 0; i < 20; ++i) {
        m.insert(i, i * 10);
    }

    EXPECT_EQ(m.size(), 20u);
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(m.at(i), i * 10);
    }
}
