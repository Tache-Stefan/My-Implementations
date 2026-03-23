#include <gtest/gtest.h>
#include <LF_Ring_Buffer.h>

TEST(LFRingBufferTest, ConstructorStartsEmpty) {
    my_std::LF_Ring_Buffer<int> rb(4);
    EXPECT_TRUE(rb.empty());
    EXPECT_FALSE(rb.full());
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.capacity(), 4);
}

TEST(LFRingBufferTest, PushIncreasesSize) {
    my_std::LF_Ring_Buffer<int> rb(4);
    EXPECT_TRUE(rb.push(10));
    EXPECT_EQ(rb.size(), 1);
    EXPECT_FALSE(rb.empty());
    EXPECT_FALSE(rb.full());
}

TEST(LFRingBufferTest, PushUntilFullThenRejects) {
    my_std::LF_Ring_Buffer<int> rb(2);
    EXPECT_TRUE(rb.push(1));
    EXPECT_TRUE(rb.push(2));
    EXPECT_TRUE(rb.full());
    EXPECT_EQ(rb.size(), 2);

    EXPECT_FALSE(rb.push(3));
    EXPECT_EQ(rb.size(), 2);
}

TEST(LFRingBufferTest, PopFromEmptyReturnsFalse) {
    my_std::LF_Ring_Buffer<int> rb(2);
    int out = 0;
    EXPECT_FALSE(rb.pop(out));
}

TEST(LFRingBufferTest, PopReturnsPushedValue) {
    my_std::LF_Ring_Buffer<int> rb(4);
    rb.push(42);

    int out = 0;
    EXPECT_TRUE(rb.pop(out));
    EXPECT_EQ(out, 42);
    EXPECT_TRUE(rb.empty());
}

TEST(LFRingBufferTest, PopPreservesFifoOrder) {
    my_std::LF_Ring_Buffer<int> rb(4);
    rb.push(1);
    rb.push(2);
    rb.push(3);

    int a = 0, b = 0, c = 0;
    EXPECT_TRUE(rb.pop(a));
    EXPECT_TRUE(rb.pop(b));
    EXPECT_TRUE(rb.pop(c));

    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
    EXPECT_TRUE(rb.empty());
}

#ifndef NDEBUG
TEST(LFRingBufferTest, ZeroCapacityAsserts) {
    EXPECT_DEATH((my_std::LF_Ring_Buffer<int>{0}), "power of two");
}

TEST(LFRingBufferTest, NonPowerOfTwoCapacityAsserts) {
    EXPECT_DEATH((my_std::LF_Ring_Buffer<int>{3}), "power of two");
}
#endif
