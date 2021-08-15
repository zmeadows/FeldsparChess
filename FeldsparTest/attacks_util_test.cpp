#include "gtest/gtest.h"

#include "unstd/macros.h"

import prelude;
import bitboard;
import board;

import attacks.util;

TEST(AttacksUtil, Attacked)
{
    Board board = {BITBOARD_EMPTY};
    Bitboard& atkrs = get_occupied_mut(board, Color::White);
    Bitboard& bishops = get_pieces_mut(board, PieceType::Bishop, Color::White);
    bishops = set_bits<a1>();
    atkrs = set_bits<a1>();

    EXPECT_EQ(bishops, BB("00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "10000000"));

    using enum Color;
    Bitboard atk = attacked(board);

    EXPECT_EQ(atk, BB("00000001"
                      "00000010"
                      "00000100"
                      "00001000"
                      "00010000"
                      "00100000"
                      "01000000"
                      "00000000"));

    bishops = set_bit(bishops, h1);
    atkrs = set_bit(atkrs, h1);

    EXPECT_EQ(bishops, BB("00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "10000001"));

    atk = attacked(board);

    EXPECT_EQ(atk, BB("10000001"
                      "01000010"
                      "00100100"
                      "00011000"
                      "00011000"
                      "00100100"
                      "01000010"
                      "00000000"));
}
