#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import prelude;
import bitboard;
import board;
import attacks.classical;

TEST(ClassicalAttacks, BishopAttackRays)
{
    {
        constexpr Square sq = 0;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("10000000"
                                        "01000000"
                                        "00100000"
                                        "00010000"
                                        "00001000"
                                        "00000100"
                                        "00000010"
                                        "00000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 7;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000001"
                                        "00000010"
                                        "00000100"
                                        "00001000"
                                        "00010000"
                                        "00100000"
                                        "01000000"
                                        "00000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 56;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000000"
                                        "00000010"
                                        "00000100"
                                        "00001000"
                                        "00010000"
                                        "00100000"
                                        "01000000"
                                        "10000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 63;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000000"
                                        "01000000"
                                        "00100000"
                                        "00010000"
                                        "00001000"
                                        "00000100"
                                        "00000010"
                                        "00000001");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 28;

        constexpr Bitboard occ = BB("00000001"
                                    "10000010"
                                    "01000100"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "01000100"
                                    "10000010");

        constexpr Bitboard attacks = BB("00000000"
                                        "00000000"
                                        "01000100"
                                        "00101000"
                                        "00000000"
                                        "00101000"
                                        "01000100"
                                        "00000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 63;

        constexpr Bitboard occ = BB("00000000"
                                    "01000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000");

        constexpr Bitboard attacks = BB("00000000"
                                        "01000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }
}

TEST(ClassicalAttacks, RookAttackRays)
{
    {
        constexpr Square sq = 0;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000001"
                                        "00000001"
                                        "00000001"
                                        "00000001"
                                        "00000001"
                                        "00000001"
                                        "00000001"
                                        "11111110");

        EXPECT_EQ(get_rook_attacks(sq, occ), attacks);
    }

    /*
    {
        constexpr Square sq = 7;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000001"
                                        "00000010"
                                        "00000100"
                                        "00001000"
                                        "00010000"
                                        "00100000"
                                        "01000000"
                                        "00000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 56;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000000"
                                        "00000010"
                                        "00000100"
                                        "00001000"
                                        "00010000"
                                        "00100000"
                                        "01000000"
                                        "10000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 63;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000000"
                                        "01000000"
                                        "00100000"
                                        "00010000"
                                        "00001000"
                                        "00000100"
                                        "00000010"
                                        "00000001");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 28;

        constexpr Bitboard occ = BB("00000001"
                                    "10000010"
                                    "01000100"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "01000100"
                                    "10000010");

        constexpr Bitboard attacks = BB("00000000"
                                        "00000000"
                                        "01000100"
                                        "00101000"
                                        "00000000"
                                        "00101000"
                                        "01000100"
                                        "00000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 63;

        constexpr Bitboard occ = BB("00000000"
                                    "01000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000");

        constexpr Bitboard attacks = BB("00000000"
                                        "01000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000");

        EXPECT_EQ(get_bishop_attacks(sq, occ), attacks);
    }
*/
}

TEST(ClassicalAttacks, QueenAttackRays)
{
    {
        constexpr Square sq = 0;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("10000001"
                                        "01000001"
                                        "00100001"
                                        "00010001"
                                        "00001001"
                                        "00000101"
                                        "00000011"
                                        "11111110");

        EXPECT_EQ(get_queen_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 7;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("10000001"
                                        "10000010"
                                        "10000100"
                                        "10001000"
                                        "10010000"
                                        "10100000"
                                        "11000000"
                                        "01111111");

        EXPECT_EQ(get_queen_attacks(sq, occ), attacks);
    }

    /*
    {
        constexpr Square sq = 56;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000000"
                                        "00000010"
                                        "00000100"
                                        "00001000"
                                        "00010000"
                                        "00100000"
                                        "01000000"
                                        "10000000");

        EXPECT_EQ(get_queen_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 63;

        constexpr Bitboard occ = BITBOARD_EMPTY;

        constexpr Bitboard attacks = BB("00000000"
                                        "01000000"
                                        "00100000"
                                        "00010000"
                                        "00001000"
                                        "00000100"
                                        "00000010"
                                        "00000001");

        EXPECT_EQ(get_queen_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 28;

        constexpr Bitboard occ = BB("00000001"
                                    "10000010"
                                    "01000100"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "01000100"
                                    "10000010");

        constexpr Bitboard attacks = BB("00000000"
                                        "00000000"
                                        "01000100"
                                        "00101000"
                                        "00000000"
                                        "00101000"
                                        "01000100"
                                        "00000000");

        EXPECT_EQ(get_queen_attacks(sq, occ), attacks);
    }

    {
        constexpr Square sq = 63;

        constexpr Bitboard occ = BB("00000000"
                                    "01000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000");

        constexpr Bitboard attacks = BB("00000000"
                                        "01000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000");

        EXPECT_EQ(get_queen_attacks(sq, occ), attacks);
    }
*/
}

TEST(ClassicalAttacks, RayBetweenSquares)
{
    {
        const Bitboard ray = ray_between_squares(0, 7);

        constexpr Bitboard bb = BB("00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "11111110");

        EXPECT_EQ(ray, bb);
    }

    {
        const Bitboard ray = ray_between_squares(0, 56);

        constexpr Bitboard bb = BB("00000001"
                                   "00000001"
                                   "00000001"
                                   "00000001"
                                   "00000001"
                                   "00000001"
                                   "00000001"
                                   "00000000");

        EXPECT_EQ(ray, bb);
    }

    {
        const Bitboard ray = ray_between_squares(0, 63);

        constexpr Bitboard bb = BB("10000000"
                                   "01000000"
                                   "00100000"
                                   "00010000"
                                   "00001000"
                                   "00000100"
                                   "00000010"
                                   "00000000");

        EXPECT_EQ(ray, bb);
    }

    {
        const Bitboard ray = ray_between_squares(9, 54);

        constexpr Bitboard bb = BB("00000000"
                                   "01000000"
                                   "00100000"
                                   "00010000"
                                   "00001000"
                                   "00000100"
                                   "00000000"
                                   "00000000");

        EXPECT_EQ(ray, bb);
    }
}

TEST(ClassicalAttacks, Attacked)
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

    Bitboard atk = attacked<false>(board, Color::White);

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

    atk = attacked<false>(board, Color::White);

    EXPECT_EQ(atk, BB("10000001"
                      "01000010"
                      "00100100"
                      "00011000"
                      "00011000"
                      "00100100"
                      "01000010"
                      "00000000"));
}
