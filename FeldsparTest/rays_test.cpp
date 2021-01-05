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
