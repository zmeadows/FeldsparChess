#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import prelude;

import move;

import <string>;

TEST_CLASS(MOVE_TEST) {
 
TEST_METHOD(TO_ALGEBRAIC)
{
    using enum PieceType;
    Assert::AreEqual(move_to_algebraic(create_quiet_move(a1, h3, 0, Pawn)), std::string("a1h3"));
    Assert::AreEqual(move_to_algebraic(create_quiet_move(b3, g6, 0, Pawn)), std::string("b3g6"));
    Assert::AreNotEqual(move_to_algebraic(create_quiet_move(b3, g6, 0, Pawn)), std::string("b4g6"));

    Assert::AreEqual(move_to_algebraic(create_capture_move(b3, f7, 0, Pawn, Pawn)), std::string("b3f7"));
    Assert::AreEqual(move_to_algebraic(create_capture_move(b3, g6, 0, Pawn, Pawn)), std::string("b3g6"));
    Assert::AreNotEqual(move_to_algebraic(create_capture_move(b3, g6, 0, Pawn, Pawn)), std::string("b4g6"));
}

};