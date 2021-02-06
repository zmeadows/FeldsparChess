#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import prelude;
import bitboard;
import print;

TEST(Prelude, CastlingRights)
{
    CastlingRights rights = 0b1111;

    remove_castling_rights(rights, CASTLE_RIGHTS_WHITE_KINGSIDE);
    EXPECT_EQ(rights, 0b1110);
    remove_castling_rights(rights, CASTLE_RIGHTS_WHITE_QUEENSIDE);
    EXPECT_EQ(rights, 0b1100);
    remove_castling_rights(rights, CASTLE_RIGHTS_BLACK_KINGSIDE);
    EXPECT_EQ(rights, 0b1000);
    remove_castling_rights(rights, CASTLE_RIGHTS_BLACK_QUEENSIDE);
    EXPECT_EQ(rights, 0b0000);

    rights = 0b1111;
    remove_castling_rights(rights, CASTLE_RIGHTS_WHITE_KINGSIDE | CASTLE_RIGHTS_WHITE_QUEENSIDE);
    EXPECT_EQ(rights, 0b1100);
    remove_castling_rights(rights, CASTLE_RIGHTS_BLACK_KINGSIDE | CASTLE_RIGHTS_BLACK_QUEENSIDE);
    EXPECT_EQ(rights, 0b0000);
}
TEST(Prelude, SquareRankFile)
{
    for (Square sq = 0; sq < 64; sq++) {
        const S64 rank = rank_of(sq);
        const S64 file = file_of(sq);
        const Square resq = from_rank_file(rank, file);
        EXPECT_EQ(sq, resq);
    }
}

TEST(Prelude, SquareFromToAlgebraic)
{
    for (Square sq = 0; sq < 64; sq++) {
        const char* alg = square_to_algebraic(sq);
        const MaybeSquare resq = square_from_algebraic(alg);
        ASSERT_TRUE(resq.has_value());
        EXPECT_EQ(sq, *resq);
    }
}

TEST(Prelude, CastlePaths)
{
    EXPECT_EQ(WHITE_KINGSIDE_CASTLE_PATH, BB("00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000110"));

    EXPECT_EQ(WHITE_QUEENSIDE_CASTLE_PATH, BB("00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "01110000"));

    EXPECT_EQ(BLACK_KINGSIDE_CASTLE_PATH, BB("00000110"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"
                                             "00000000"));

    EXPECT_EQ(BLACK_QUEENSIDE_CASTLE_PATH, BB("01110000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"
                                              "00000000"));
}
