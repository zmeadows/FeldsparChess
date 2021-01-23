#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import unstd.core;
import unstd.string;

TEST(String, Constructors)
{
    String s("asdf");
    EXPECT_EQ(s, "asdf");

    char* ch = (char*)malloc(sizeof(char) * 8);
    ch[7] = '\0';

    String s2(ch);
}
