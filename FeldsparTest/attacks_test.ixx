#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import prelude;

import attacks.rays;
import attacks.util;
import attacks;
import bitboard;
import board;
import init;
import print;

import <string>;

TEST_CLASS(ATTACK_TEST) {

TEST_CLASS_INITIALIZE(FELDSPAR_INIT) {
    feldspar_init();
}
 
TEST_METHOD(KNIGHT_MOVES)
{
    for (Square from_sq = 0; from_sq < 64; from_sq++) {
        const Bitboard km = get_knight_moves(from_sq);

        const S64 rank = rank_of(from_sq);
        const S64 file = file_of(from_sq);

        Bitboard km2 = BITBOARD_EMPTY;
        for (S64 to_sq : {from_sq + 10, from_sq + 17, from_sq + 6, from_sq + 15, from_sq - 10,
                          from_sq - 17, from_sq - 6, from_sq - 15}) {
            if (to_sq < 0 || to_sq >= 64) continue;
            if (abs(rank_of(to_sq) - rank) > 2) continue;
            if (abs(file_of(to_sq) - file) > 2) continue;

            km2 = set_bit(km2, to_sq);
        }

        Assert::AreEqual(km, km2);
    }
}

TEST_METHOD(ATTACKED_FN)
{
    Board board = {BITBOARD_EMPTY};
    Bitboard& atkrs = get_occupied_mut(board, Color::White);
    Bitboard& bishops = get_pieces_mut(board, PieceType::Bishop, Color::White);
    bishops = set_bits<a1>();
    atkrs = set_bits<a1>();

    Assert::AreEqual(bishops, BB("00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "10000000"));

    using enum Color;
    Bitboard atk = attacked(board);

    Assert::AreEqual(atk, BB("00000001"
                      "00000010"
                      "00000100"
                      "00001000"
                      "00010000"
                      "00100000"
                      "01000000"
                      "00000000"));

    bishops = set_bit(bishops, h1);
    atkrs = set_bit(atkrs, h1);

    Assert::AreEqual(bishops, BB("00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "00000000"
                          "10000001"));

    atk = attacked(board);

    Assert::AreEqual(atk, BB("10000001"
                      "01000010"
                      "00100100"
                      "00011000"
                      "00011000"
                      "00100100"
                      "01000010"
                      "00000000"));
}

TEST_METHOD(BISHOP_ATTACKS)
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
    }
}

TEST_METHOD(ROOK_ATTACKS)
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

        Assert::AreEqual(get_rook_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_bishop_attacks(sq, occ), attacks);
    }
*/
}

TEST_METHOD(QUEEN_ATTACKS)
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

        Assert::AreEqual(get_queen_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_queen_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_queen_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_queen_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_queen_attacks(sq, occ), attacks);
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

        Assert::AreEqual(get_queen_attacks(sq, occ), attacks);
    }
*/
}

TEST_METHOD(RAY_BETWEEN_SQUARES)
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

        Assert::AreEqual(ray, bb);
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

        Assert::AreEqual(ray, bb);
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

        Assert::AreEqual(ray, bb);
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

        Assert::AreEqual(ray, bb);
    }
}

};
