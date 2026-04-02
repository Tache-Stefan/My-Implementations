#include <gtest/gtest.h>
#include <string>
#include <type_traits>
#include <Unique_Ptr.h>

namespace {

struct Tracker {
    static int constructions;
    static int destructions;

    int value;
    explicit Tracker(int v = 0) : value(v) { ++constructions; }
    ~Tracker() { ++destructions; }
};

int Tracker::constructions = 0;
int Tracker::destructions = 0;

void reset_tracker_counts() {
    Tracker::constructions = 0;
    Tracker::destructions = 0;
}

}

static_assert(!std::is_copy_constructible_v<my_std::Unique_Ptr<int>>);
static_assert(!std::is_copy_assignable_v<my_std::Unique_Ptr<int>>);
static_assert(std::is_move_constructible_v<my_std::Unique_Ptr<int>>);
static_assert(std::is_move_assignable_v<my_std::Unique_Ptr<int>>);

TEST(UniquePtrTest, DefaultConstructorIsNull) {
    my_std::Unique_Ptr<int> p;
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);
}

TEST(UniquePtrTest, BoolAndDereferenceOperators) {
    my_std::Unique_Ptr<int> p(new int(42));
    EXPECT_TRUE(p);
    EXPECT_EQ(*p, 42);
    *p = 99;
    EXPECT_EQ(*p, 99);
}

TEST(UniquePtrTest, ArrowOperatorWorks) {
    my_std::Unique_Ptr<std::string> p(new std::string("hello"));
    EXPECT_EQ(p->size(), 5u);
    EXPECT_EQ(*p, "hello");
}

TEST(UniquePtrTest, MoveConstructorTransfersOwnership) {
    my_std::Unique_Ptr<int> src(new int(7));
    my_std::Unique_Ptr<int> dst(std::move(src));

    EXPECT_FALSE(src);
    EXPECT_TRUE(dst);
    EXPECT_EQ(*dst, 7);
}

TEST(UniquePtrTest, MoveAssignmentTransfersOwnershipAndDeletesOld) {
    reset_tracker_counts();

    {
        my_std::Unique_Ptr<Tracker> src(new Tracker(1));
        my_std::Unique_Ptr<Tracker> dst(new Tracker(2));

        dst = std::move(src);

        EXPECT_FALSE(src);
        EXPECT_TRUE(dst);
        EXPECT_EQ(dst->value, 1);
        EXPECT_EQ(Tracker::destructions, 1); // old dst object deleted
    }

    EXPECT_EQ(Tracker::constructions, 2);
    EXPECT_EQ(Tracker::destructions, 2);
}

TEST(UniquePtrTest, SelfMoveAssignmentIsSafe) {
    my_std::Unique_Ptr<int> p(new int(123));
    p = std::move(p);

    EXPECT_TRUE(p);
    EXPECT_EQ(*p, 123);
}

TEST(UniquePtrTest, ReleaseReturnsPointerAndDisowns) {
    reset_tracker_counts();

    Tracker* raw = nullptr;
    {
        my_std::Unique_Ptr<Tracker> p(new Tracker(5));
        raw = p.release();

        EXPECT_FALSE(p);
        EXPECT_NE(raw, nullptr);
        EXPECT_EQ(raw->value, 5);
        EXPECT_EQ(Tracker::destructions, 0);
    }

    EXPECT_EQ(Tracker::destructions, 0);
    delete raw;
    EXPECT_EQ(Tracker::destructions, 1);
}

TEST(UniquePtrTest, ResetDeletesOldObject) {
    reset_tracker_counts();

    {
        my_std::Unique_Ptr<Tracker> p(new Tracker(10));
        p.reset(new Tracker(20));

        EXPECT_TRUE(p);
        EXPECT_EQ(p->value, 20);
        EXPECT_EQ(Tracker::destructions, 1);
    }

    EXPECT_EQ(Tracker::constructions, 2);
    EXPECT_EQ(Tracker::destructions, 2);
}

TEST(UniquePtrTest, ResetToNullDeletesOwnedObject) {
    reset_tracker_counts();

    {
        my_std::Unique_Ptr<Tracker> p(new Tracker(33));
        p.reset();

        EXPECT_FALSE(p);
        EXPECT_EQ(Tracker::destructions, 1);
    }

    EXPECT_EQ(Tracker::constructions, 1);
    EXPECT_EQ(Tracker::destructions, 1);
}

TEST(UniquePtrTest, ResetSamePointerDoesNotDoubleDelete) {
    reset_tracker_counts();

    Tracker* raw = new Tracker(11);
    {
        my_std::Unique_Ptr<Tracker> p(raw);
        p.reset(raw);
        EXPECT_EQ(Tracker::destructions, 0);
    }

    EXPECT_EQ(Tracker::constructions, 1);
    EXPECT_EQ(Tracker::destructions, 1);
}

TEST(UniquePtrTest, ArraySpecializationBracketAccess) {
    my_std::Unique_Ptr<int[]> p(new int[3]{1, 2, 3});
    EXPECT_TRUE(p);
    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p[1], 2);
    EXPECT_EQ(p[2], 3);

    p[1] = 20;
    EXPECT_EQ(p[1], 20);
}

TEST(UniquePtrTest, ArrayMoveConstructorTransfersOwnership) {
    my_std::Unique_Ptr<int[]> src(new int[2]{4, 5});
    my_std::Unique_Ptr<int[]> dst(std::move(src));

    EXPECT_FALSE(src);
    EXPECT_TRUE(dst);
    EXPECT_EQ(dst[0], 4);
    EXPECT_EQ(dst[1], 5);
}

TEST(UniquePtrTest, ArrayReleaseReturnsPointerAndDisowns) {
    int* raw = nullptr;
    {
        my_std::Unique_Ptr<int[]> p(new int[2]{9, 8});
        raw = p.release();

        EXPECT_FALSE(p);
        EXPECT_NE(raw, nullptr);
        EXPECT_EQ(raw[0], 9);
        EXPECT_EQ(raw[1], 8);
    }

    delete[] raw;
}

TEST(UniquePtrTest, ArrayResetReplacesBuffer) {
    my_std::Unique_Ptr<int[]> p(new int[2]{1, 2});
    p.reset(new int[3]{7, 8, 9});

    EXPECT_TRUE(p);
    EXPECT_EQ(p[0], 7);
    EXPECT_EQ(p[1], 8);
    EXPECT_EQ(p[2], 9);
}

TEST(UniquePtrTest, MakeUniqueForObject) {
    auto p = my_std::make_unique<std::string>(5, 'a');
    EXPECT_TRUE(p);
    EXPECT_EQ(*p, "aaaaa");
}

TEST(UniquePtrTest, MakeUniqueForArrayValueInitializesElements) {
    auto p = my_std::make_unique<int[]>(4);
    EXPECT_TRUE(p);

    for (std::size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(p[i], 0);
    }
}
