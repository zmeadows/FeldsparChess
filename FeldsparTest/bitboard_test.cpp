#include "gtest/gtest.h"
#include "init_fixture.h"
#include "unstd/macros.h"

import unstd.core;

import prelude;
import bitboard;

class BitboardTest : public InitFixture {};

TEST_METHOD(BitboardTest, SetBits)
{
    {
        constexpr Bitboard bb = set_bits<h1, a1>();
        Assert::AreEqual(bb, BB("00000000"
                         "00000000"
                         "00000000"
                         "00000000"
                         "00000000"
                         "00000000"
                         "00000000"
                         "10000001"));
    }

    {
        constexpr Bitboard bb = set_bits<h1, a1, h8, a8>();
        Assert::AreEqual(bb, BB("10000001"
                         "00000000"
                         "00000000"
                         "00000000"
                         "00000000"
                         "00000000"
                         "00000000"
                         "10000001"));
    }

    {
        constexpr Bitboard bb = set_bits<a8, a7, a6, a5, a4, a3, a2, a1>();
        Assert::AreEqual(bb, BB("10000000"
                         "10000000"
                         "10000000"
                         "10000000"
                         "10000000"
                         "10000000"
                         "10000000"
                         "10000000"));
    }
}

TEST_METHOD(BitboardTest, StringToBitboardConversion)
{
    Assert::AreEqual(BITBOARD_EMPTY, BB("00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"));

    Assert::AreEqual(BITBOARD_FULL, BB("11111111"
                                "11111111"
                                "11111111"
                                "11111111"
                                "11111111"
                                "11111111"
                                "11111111"
                                "11111111"));

    Assert::AreEqual(square_bitrep(0), BB("00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "00000000"
                                   "00000001"));

    Assert::AreEqual(square_bitrep(63), BB("10000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"
                                    "00000000"));

    Assert::AreEqual(0b1001011110010111100101111001011110010111100101111001011110010111, BB("10010111"
                                                                                     "10010111"
                                                                                     "10010111"
                                                                                     "10010111"
                                                                                     "10010111"
                                                                                     "10010111"
                                                                                     "10010111"
                                                                                     "10010111"));

    Assert::AreEqual(0b0100110000010101010001000010111011100000010010101001100011001001, BB("01001100"
                                                                                     "00010101"
                                                                                     "01000100"
                                                                                     "00101110"
                                                                                     "11100000"
                                                                                     "01001010"
                                                                                     "10011000"
                                                                                     "11001001"));
}

TEST_METHOD(BitboardTest, BitScanForward)
{

    Assert::AreEqual(0, bitboard_bsf(BB("00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000")));

    Assert::AreEqual(0, bitboard_bsf(BB("00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000001")));

    Assert::AreEqual(31, bitboard_bsf(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "10000000"
                                  "00000000"
                                  "00000000"
                                  "00000000")));

    Assert::AreEqual(7, bitboard_bsf(BB("00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "10000000")));

    Assert::AreEqual(7, bitboard_bsf(BB("10000000"
                                 "00010000"
                                 "01001110"
                                 "00000000"
                                 "01000000"
                                 "00001110"
                                 "00000000"
                                 "10000000")));

    Assert::AreEqual(63, bitboard_bsf(BB("10000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000")));
}

TEST_METHOD(BitboardTest, BitScanReverse)
{

    Assert::AreEqual(0, bitboard_bsr(BB("00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000")));

    Assert::AreEqual(0, bitboard_bsr(BB("00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000001")));

    Assert::AreEqual(31, bitboard_bsr(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "10000000"
                                  "00000000"
                                  "00000000"
                                  "00000000")));

    Assert::AreEqual(7, bitboard_bsr(BB("00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "00000000"
                                 "10000000")));

    Assert::AreEqual(63, bitboard_bsr(BB("10000000"
                                  "00010000"
                                  "01001110"
                                  "00000000"
                                  "01000000"
                                  "00001110"
                                  "00000000"
                                  "10000000")));

    Assert::AreEqual(63, bitboard_bsr(BB("10000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000")));
}

TEST_METHOD(BitboardTest, PopulationCount)
{

    Assert::AreEqual(1, bitboard_popcount(BB("10000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000")));

    Assert::AreEqual(0, bitboard_popcount(BB("00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000"
                                      "00000000")));

    Assert::AreEqual(8, bitboard_popcount(BB("00010000"
                                      "00010000"
                                      "00010000"
                                      "00010000"
                                      "00010000"
                                      "00010000"
                                      "00010000"
                                      "00010000")));

    Assert::AreEqual(64, bitboard_popcount(BB("11111111"
                                       "11111111"
                                       "11111111"
                                       "11111111"
                                       "11111111"
                                       "11111111"
                                       "11111111"
                                       "11111111")));

    Assert::AreEqual(54, bitboard_popcount(BB("11101111"
                                       "10111011"
                                       "11111111"
                                       "10101111"
                                       "11111101"
                                       "11111111"
                                       "11110111"
                                       "10101101")));

    Assert::AreEqual(44, bitboard_popcount(BB("11101001"
                                       "10111011"
                                       "11001111"
                                       "10101111"
                                       "11111101"
                                       "10010011"
                                       "10110111"
                                       "10101001")));
}

TEST_METHOD(BitboardTest, EmptyFullOccupiedChecks)
{
    EXPECT_TRUE(bitboard_is_full(BITBOARD_FULL));

    EXPECT_TRUE(bitboard_is_full(BB("11111111"
                                    "11111111"
                                    "11111111"
                                    "11111111"
                                    "11111111"
                                    "11111111"
                                    "11111111"
                                    "11111111")));

    EXPECT_TRUE(bitboard_is_empty(BITBOARD_EMPTY));

    EXPECT_TRUE(bitboard_is_empty(BB("00000000"
                                     "00000000"
                                     "00000000"
                                     "00000000"
                                     "00000000"
                                     "00000000"
                                     "00000000"
                                     "00000000")));

    EXPECT_TRUE(bitboard_is_occupied(BB("00000000"
                                        "00000000"
                                        "00000000"
                                        "00000000"
                                        "00100000"
                                        "00000000"
                                        "00000000"
                                        "00000000")));

    EXPECT_TRUE(bitboard_is_occupied(BITBOARD_FULL));
    EXPECT_TRUE(!bitboard_is_occupied(BITBOARD_EMPTY));
}

TEST_METHOD(BitboardTest, Shifts)
{
    Assert::AreEqual(BITBOARD_EMPTY, bitboard_shifted(BB("11111111"
                                                  "00000000"
                                                  "00000000"
                                                  "00000000"
                                                  "00000000"
                                                  "00000000"
                                                  "00000000"
                                                  "00000000"),
                                               Direction::North));

    Assert::AreEqual(bitboard_shifted(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "11111111"),
                               Direction::North),
              BB("00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "11111111"
                 "00000000"));

    Assert::AreEqual(BB("00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "11111111"),
              bitboard_shifted(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "11111111"
                                  "00000000"),
                               Direction::South));

    Assert::AreEqual(bitboard_shifted(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "11111111"),
                               Direction::South),
              BITBOARD_EMPTY);

    Assert::AreEqual(bitboard_shifted(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "11111111"),
                               Direction::NorthEast),
              BB("00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "01111111"
                 "00000000"));

    Assert::AreEqual(bitboard_shifted(BB("10000000"
                                  "01000000"
                                  "00100000"
                                  "00010000"
                                  "00001000"
                                  "00000100"
                                  "00000010"
                                  "00000001"),
                               Direction::NorthEast),
              BB("00100000"
                 "00010000"
                 "00001000"
                 "00000100"
                 "00000010"
                 "00000001"
                 "00000000"
                 "00000000"));

    Assert::AreEqual(bitboard_shifted(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "11111111"),
                               Direction::NorthWest),
              BB("00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "00000000"
                 "11111110"
                 "00000000"));

    Assert::AreEqual(bitboard_shifted(BB("10000000"
                                  "01000000"
                                  "00100000"
                                  "00010000"
                                  "00001000"
                                  "00000100"
                                  "00000010"
                                  "00000001"),
                               Direction::NorthWest),
              BB("10000000"
                 "01000000"
                 "00100000"
                 "00010000"
                 "00001000"
                 "00000100"
                 "00000010"
                 "00000000"));
}

TEST_METHOD(BitboardTest, Flip)
{
    Assert::AreEqual(bitboard_flipped(BB("00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "00000000"
                                  "11111111")),
              BB("11111111"
                 "11111111"
                 "11111111"
                 "11111111"
                 "11111111"
                 "11111111"
                 "11111111"
                 "00000000"));

    Assert::AreEqual(bitboard_flipped(BB("11111111"
                                  "10000001"
                                  "10000001"
                                  "10011001"
                                  "10011001"
                                  "10000001"
                                  "10000001"
                                  "11111111")),
              BB("00000000"
                 "01111110"
                 "01111110"
                 "01100110"
                 "01100110"
                 "01111110"
                 "01111110"
                 "00000000"));

    Assert::AreEqual(bitboard_flipped(BITBOARD_EMPTY), BITBOARD_FULL);
    Assert::AreEqual(bitboard_flipped(BITBOARD_FULL), BITBOARD_EMPTY);
}

TEST_METHOD(BitboardTest, Serialization)
{
    {
        const Bitboard bb = BB("01100000"
                               "00010000"
                               "00010000"
                               "00001000"
                               "00000100"
                               "00000100"
                               "00000000"
                               "00000001");
        U64 count = 0;
        serialize(bb, [&](Square sq) -> void { count++; });
        EXPECT_TRUE(count == 8);
    }

    {
        U64 count = 0;
        serialize(BITBOARD_EMPTY, [&](Square sq) -> void { count++; });
        EXPECT_TRUE(count == 0);
    }

    {
        const Bitboard bb = BB("10000000"
                               "01000000"
                               "00100000"
                               "00010000"
                               "00001000"
                               "00000100"
                               "00000010"
                               "00000001");
        std::vector<Square> squares;
        serialize(bb, [&](Square sq) -> void { squares.push_back(sq); });
        const std::vector<Square> result = {0, 9, 18, 27, 36, 45, 54, 63};
        Assert::AreEqual(squares, result);
    }
}

TEST_METHOD(BitboardTest, RankMask)
{
    for (Square sq = 0; sq < 8; sq++) {
        Assert::AreEqual(rank_mask(sq), FIRST_RANK);
    }

    for (Square sq = 8; sq < 16; sq++) {
        Assert::AreEqual(rank_mask(sq), SECOND_RANK);
    }

    for (Square sq = 16; sq < 24; sq++) {
        Assert::AreEqual(rank_mask(sq), THIRD_RANK);
    }

    for (Square sq = 24; sq < 32; sq++) {
        Assert::AreEqual(rank_mask(sq), FOURTH_RANK);
    }

    for (Square sq = 32; sq < 40; sq++) {
        Assert::AreEqual(rank_mask(sq), FIFTH_RANK);
    }

    for (Square sq = 40; sq < 48; sq++) {
        Assert::AreEqual(rank_mask(sq), SIXTH_RANK);
    }

    for (Square sq = 48; sq < 56; sq++) {
        Assert::AreEqual(rank_mask(sq), SEVENTH_RANK);
    }

    for (Square sq = 56; sq < 64; sq++) {
        Assert::AreEqual(rank_mask(sq), EIGTH_RANK);
    }
}

TEST_METHOD(BitboardTest, FileMask)
{
    for (Square sq = 0; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), H_FILE);
    }

    for (Square sq = 1; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), G_FILE);
    }

    for (Square sq = 2; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), F_FILE);
    }

    for (Square sq = 3; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), E_FILE);
    }

    for (Square sq = 4; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), D_FILE);
    }

    for (Square sq = 5; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), C_FILE);
    }

    for (Square sq = 6; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), B_FILE);
    }

    for (Square sq = 7; sq < 64; sq += 8) {
        Assert::AreEqual(file_mask(sq), A_FILE);
    }
}
