#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import prelude;

import bitboard;

TEST_CLASS(PRELUDE_TEST) {

TEST_METHOD(CASTLING_RIGHTS)
{
    CastlingRights rights = 0b1111;

    remove_castling_rights(rights, CASTLE_RIGHTS_WHITE_KINGSIDE);
    Assert::AreEqual(rights, (CastlingRights)0b1110);
    remove_castling_rights(rights, CASTLE_RIGHTS_WHITE_QUEENSIDE);
    Assert::AreEqual(rights, (CastlingRights)0b1100);
    remove_castling_rights(rights, CASTLE_RIGHTS_BLACK_KINGSIDE);
    Assert::AreEqual(rights, (CastlingRights)0b1000);
    remove_castling_rights(rights, CASTLE_RIGHTS_BLACK_QUEENSIDE);
    Assert::AreEqual(rights, (CastlingRights)0b0000);

    rights = 0b1111;
    remove_castling_rights(rights, CASTLE_RIGHTS_WHITE_KINGSIDE | CASTLE_RIGHTS_WHITE_QUEENSIDE);
    Assert::AreEqual(rights, (CastlingRights)0b1100);
    remove_castling_rights(rights, CASTLE_RIGHTS_BLACK_KINGSIDE | CASTLE_RIGHTS_BLACK_QUEENSIDE);
    Assert::AreEqual(rights, (CastlingRights)0b0000);
}

TEST_METHOD(SQUARE_RANK_FILE)
{
    for (Square sq = 0; sq < 64; sq++) {
        const S64 rank = rank_of(sq);
        const S64 file = file_of(sq);
        const Square resq = from_rank_file(rank, file);
        Assert::AreEqual(sq, resq);
    }
}

TEST_METHOD(SQUARE_ALGEBRAIC)
{
    for (Square in_sq = 0; in_sq < 64; in_sq++) {
        const char* alg = square_to_algebraic(in_sq);
        const MaybeSquare out_sq = square_from_algebraic(alg);
        Assert::IsTrue(out_sq.has_value());
        Assert::AreEqual(in_sq, *out_sq);
    }
}

TEST_METHOD(CASTLE_PATHS)
{
    Assert::AreEqual(WHITE_KINGSIDE_CASTLE_PATH, BB("00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000110"));

    Assert::AreEqual(WHITE_QUEENSIDE_CASTLE_PATH, BB("00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "01110000"));

    Assert::AreEqual(BLACK_KINGSIDE_CASTLE_PATH, BB("00000110"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"
                                                    "00000000"));

    Assert::AreEqual(BLACK_QUEENSIDE_CASTLE_PATH, BB("01110000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"
                                                     "00000000"));
}

};

