#include <gtest/gtest.h>
#include <Ring_Buffer.h>
#include <string>

TEST(RingBufferTest, DefaultConstructorIsEmpty) {
    my_std::Ring_Buffer<int, 4> rb;
    EXPECT_TRUE(rb.empty());
    EXPECT_FALSE(rb.full());
    EXPECT_EQ(rb.size(), 0);
}

TEST(RingBufferTest, TryPushSingleElement) {
    my_std::Ring_Buffer<int, 4> rb;
    rb.try_push(42);
    EXPECT_EQ(rb.size(), 1);
    EXPECT_EQ(rb.front(), 42);
    EXPECT_EQ(rb.back(), 42);
}

TEST(RingBufferTest, TryPushToFull) {
    my_std::Ring_Buffer<int, 3> rb;
    rb.try_push(1);
    rb.try_push(2);
    rb.try_push(3);
    EXPECT_TRUE(rb.full());
    EXPECT_EQ(rb.size(), 3);
    EXPECT_EQ(rb.front(), 1);
    EXPECT_EQ(rb.back(), 3);
}

TEST(RingBufferTest, TryPushDropsWhenFull) {
    my_std::Ring_Buffer<int, 2> rb;
    rb.try_push(1);
    rb.try_push(2);
    rb.try_push(3); // should be dropped
    EXPECT_EQ(rb.size(), 2);
    EXPECT_EQ(rb.front(), 1);
    EXPECT_EQ(rb.back(), 2);
}

TEST(RingBufferTest, TryPushRvalue) {
    my_std::Ring_Buffer<std::string, 4> rb;
    std::string s = "hello";
    rb.try_push(std::move(s));
    EXPECT_EQ(rb.front(), "hello");
}

TEST(RingBufferTest, PushSingleElement) {
    my_std::Ring_Buffer<int, 4> rb;
    rb.push(10);
    EXPECT_EQ(rb.size(), 1);
    EXPECT_EQ(rb.front(), 10);
    EXPECT_EQ(rb.back(), 10);
}

TEST(RingBufferTest, PushOverwritesOldest) {
    my_std::Ring_Buffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);
    EXPECT_TRUE(rb.full());
    rb.push(4); // overwrites 1
    EXPECT_EQ(rb.size(), 3);
    EXPECT_EQ(rb.front(), 2);
    EXPECT_EQ(rb.back(), 4);
}

TEST(RingBufferTest, PushOverwritesMultiple) {
    my_std::Ring_Buffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4); // overwrites 1
    rb.push(5); // overwrites 2
    EXPECT_EQ(rb.size(), 3);
    EXPECT_EQ(rb.front(), 3);
    EXPECT_EQ(rb.back(), 5);
}

TEST(RingBufferTest, PushRvalue) {
    my_std::Ring_Buffer<std::string, 2> rb;
    std::string s = "world";
    rb.push(std::move(s));
    EXPECT_EQ(rb.front(), "world");
}

TEST(RingBufferTest, PopSingleElement) {
    my_std::Ring_Buffer<int, 4> rb;
    rb.push(1);
    rb.pop();
    EXPECT_TRUE(rb.empty());
    EXPECT_EQ(rb.size(), 0);
}

TEST(RingBufferTest, PopFromEmpty) {
    my_std::Ring_Buffer<int, 4> rb;
    rb.pop();
    EXPECT_TRUE(rb.empty());
}

TEST(RingBufferTest, PopAdvancesFront) {
    my_std::Ring_Buffer<int, 4> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.pop();
    EXPECT_EQ(rb.front(), 2);
    EXPECT_EQ(rb.size(), 2);
    rb.pop();
    EXPECT_EQ(rb.front(), 3);
    EXPECT_EQ(rb.size(), 1);
}

TEST(RingBufferTest, FrontAndBackSameWhenOneElement) {
    my_std::Ring_Buffer<int, 4> rb;
    rb.push(99);
    EXPECT_EQ(rb.front(), 99);
    EXPECT_EQ(rb.back(), 99);
}

TEST(RingBufferTest, WraparoundWithTryPush) {
    my_std::Ring_Buffer<int, 3> rb;
    rb.try_push(1);
    rb.try_push(2);
    rb.try_push(3);
    rb.pop(); // removes 1, front wraps forward
    rb.pop(); // removes 2
    rb.try_push(4);
    rb.try_push(5);
    EXPECT_EQ(rb.size(), 3);
    EXPECT_EQ(rb.front(), 3);
    EXPECT_EQ(rb.back(), 5);
}

TEST(RingBufferTest, WraparoundWithPush) {
    my_std::Ring_Buffer<int, 3> rb;
    for (int i = 1; i <= 6; ++i) {
        rb.push(i); // continuously overwrite
    }
    EXPECT_EQ(rb.size(), 3);
    EXPECT_EQ(rb.front(), 4);
    EXPECT_EQ(rb.back(), 6);
}

TEST(RingBufferTest, WraparoundPopAll) {
    my_std::Ring_Buffer<int, 3> rb;
    for (int i = 1; i <= 5; ++i) rb.push(i);
    rb.pop();
    rb.pop();
    rb.pop();
    EXPECT_TRUE(rb.empty());
}

TEST(RingBufferTest, CapacityOne) {
    my_std::Ring_Buffer<int, 1> rb;
    rb.push(1);
    EXPECT_TRUE(rb.full());
    EXPECT_EQ(rb.front(), 1);
    EXPECT_EQ(rb.back(), 1);
    rb.push(2); // overwrites 1
    EXPECT_EQ(rb.front(), 2);
    rb.pop();
    EXPECT_TRUE(rb.empty());
}


TEST(RingBufferTest, InterleavedPushPop) {
    my_std::Ring_Buffer<int, 4> rb;
    rb.push(1);
    rb.push(2);
    rb.pop();       // removes 1
    rb.push(3);
    rb.push(4);
    rb.pop();       // removes 2
    rb.push(5);
    EXPECT_EQ(rb.size(), 3);
    EXPECT_EQ(rb.front(), 3);
    EXPECT_EQ(rb.back(), 5);
}

TEST(RingBufferTest, StringType) {
    my_std::Ring_Buffer<std::string, 3> rb;
    rb.push("alpha");
    rb.push("beta");
    rb.push("gamma");
    rb.push("delta"); // overwrites "alpha"
    EXPECT_EQ(rb.front(), "beta");
    EXPECT_EQ(rb.back(), "delta");
    EXPECT_EQ(rb.size(), 3);
}
