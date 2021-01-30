#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import prelude;
import move;

TEST(Move, ToAlgebraic)
{
    using enum PieceType;
    EXPECT_EQ(move_to_algebraic(create_quiet_move(a1, h3, 0, Pawn)), "a1h3");
    EXPECT_EQ(move_to_algebraic(create_quiet_move(b3, g6, 0, Pawn)), "b3g6");
    EXPECT_NE(move_to_algebraic(create_quiet_move(b3, g6, 0, Pawn)), "b4g6");
    EXPECT_EQ(move_to_algebraic(create_capture_move(a1, h3, 0, Pawn, Pawn)), "a1h3");
    EXPECT_EQ(move_to_algebraic(create_capture_move(b3, g6, 0, Pawn, Pawn)), "b3g6");
    EXPECT_NE(move_to_algebraic(create_capture_move(b3, g6, 0, Pawn, Pawn)), "b4g6");
}
