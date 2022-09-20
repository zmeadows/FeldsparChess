#include "gtest/gtest.h"
#include "init_fixture.h"
import unstd.core;

import prelude;
import move;

class MoveTest : public InitFixture {};

TEST_METHOD(MoveTest, ToAlgebraic)
{
    using enum PieceType;
    Assert::AreEqual(move_to_algebraic(create_quiet_move(a1, h3, 0, Pawn)), "a1h3");
    Assert::AreEqual(move_to_algebraic(create_quiet_move(b3, g6, 0, Pawn)), "b3g6");
    EXPECT_NE(move_to_algebraic(create_quiet_move(b3, g6, 0, Pawn)), "b4g6");

    Assert::AreEqual(move_to_algebraic(create_capture_move(b3, f7, 0, Pawn, Pawn)), "b3f7");
    Assert::AreEqual(move_to_algebraic(create_capture_move(b3, g6, 0, Pawn, Pawn)), "b3g6");
    EXPECT_NE(move_to_algebraic(create_capture_move(b3, g6, 0, Pawn, Pawn)), "b4g6");
}
