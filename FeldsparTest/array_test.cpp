#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import unstd.core;
import unstd.array;

TEST(Array, Constructors)
{
    {
        constexpr Array<S64, 10> arr(-123);

        for (U64 i = 0; i < arr.length(); i++) {
            EXPECT_EQ(arr[i], -123);
        }
    }

    {
        constexpr Array<U64, 10> arr({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

        for (U64 i = 0; i < arr.length(); i++) {
            EXPECT_EQ(arr[i], i);
        }
    }
}

TEST(DynArray, StackHeapCapacity)
{
    DynArray<U64, 32> sarr;
    EXPECT_EQ(sarr.capacity(), 32);

    DynArray<U64> harr;
    EXPECT_EQ(harr.capacity(), 0);

    EXPECT_GE(sizeof(sarr), sizeof(harr) + 32 * sizeof(U64) + sizeof(bool));
}

TEST(DynArray, OnStack)
{
    DynArray<U64, 32> arr;
    EXPECT_TRUE(arr.on_stack());
    for (U64 i = 0; i < 32; i++) {
        arr.append(i * i);
    }
    EXPECT_TRUE(arr.on_stack());
    for (auto i = 0; i < 32; i++) {
        EXPECT_EQ(arr[i], i * i);
    }

    EXPECT_TRUE(arr.on_stack());
    arr.append(0);
    EXPECT_FALSE(arr.on_stack());

    EXPECT_EQ(arr.capacity(), 64);

    for (auto i = 0; i < 32; i++) {
        EXPECT_EQ(arr[i], i * i);
    }

    EXPECT_EQ(arr[32], 0);

    arr.clear();

    EXPECT_TRUE(arr.on_stack());
    EXPECT_TRUE(arr.capacity() == 32);
}
