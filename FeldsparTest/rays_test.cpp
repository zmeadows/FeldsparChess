#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import prelude;
import bitboard;
import rays;

TEST(Rays, BishopAttackRays)
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

TEST(Rays, RookAttackRays)
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

TEST(Rays, QueenAttackRays)
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

TEST(Rays, RayBetweenSquares)
{
    {
        const Bitboard ray = get_ray_between_squares(0, 7);

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
        const Bitboard ray = get_ray_between_squares(0, 56);

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
        const Bitboard ray = get_ray_between_squares(0, 63);

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
        const Bitboard ray = get_ray_between_squares(9, 54);

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
