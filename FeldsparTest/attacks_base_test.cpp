#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import unstd.core;

import prelude;
import bitboard;
import print;

import attacks.base;

TEST_METHOD(AttacksBase, KnightMoves)
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

        print_bitboard(km);
        print_bitboard(km2);
        Assert::AreEqual(km, km2);
    }
}
