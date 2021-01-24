#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import unstd.core;
import unstd.string;

TEST(String, Constructors)
{
    String s("asdf");
    EXPECT_EQ(s, "asdf");
}

TEST(StringRef, Constructors)
{
    String str("Whatever");
    StringRef ref(str);

    EXPECT_EQ(ref, "Whatever");
}

TEST(String, Split)
{
    const String sentence("This is a test sentence.");
    const auto words = split<10>(sentence);

    EXPECT_EQ(words.length(), 5);

    EXPECT_EQ(words[0], "This");
    EXPECT_EQ(words[1], "is");
    EXPECT_EQ(words[2], "a");
    EXPECT_EQ(words[3], "test");
    EXPECT_EQ(words[4], "sentence.");
}
