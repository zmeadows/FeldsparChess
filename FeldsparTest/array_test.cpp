#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import unstd.core;
import unstd.array;

TEST(DynArray, Append)
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
}
