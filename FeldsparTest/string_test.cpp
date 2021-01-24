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
    {
        const String sentence("This is a test sentence.");
        const auto words = split<5>(sentence);

        EXPECT_EQ(words.length(), 5);

        EXPECT_EQ(words[0], "This");
        EXPECT_EQ(words[1], "is");
        EXPECT_EQ(words[2], "a");
        EXPECT_EQ(words[3], "test");
        EXPECT_EQ(words[4], "sentence.");
    }

    {
        const String fen("3q1r1k/2r2ppp/2p1b3/1p2P2R/p1pP2Nb/P3Q2P/1P2B1P1/5RK1 w - - 5 27");
        const auto words = split<6>(fen);

        EXPECT_EQ(words.length(), 6);

        EXPECT_EQ(words[0], "3q1r1k/2r2ppp/2p1b3/1p2P2R/p1pP2Nb/P3Q2P/1P2B1P1/5RK1");
        EXPECT_EQ(words[1], "w");
        EXPECT_EQ(words[2], "-");
        EXPECT_EQ(words[3], "-");
        EXPECT_EQ(words[4], "5");
        EXPECT_EQ(words[5], "27");
    }
}
