#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import prelude;

import bitboard;
import board;
import game;
import init;

import <string>;

TEST_CLASS(GAME_TEST)
{

TEST_CLASS_INITIALIZE(FELDSPAR_INIT) {
    feldspar_init();
}

    TEST_METHOD(INIT)
    {
        // default 'empty' game state
        Game g;

        for (const PieceType p : EnumRange<PieceType>()) {
            for (const Color c : EnumRange<Color>()) {
                const Bitboard bb = get_pieces(g.board, p, c);
                Assert::AreEqual(bb, BITBOARD_EMPTY);
            }
        }

        Assert::IsFalse(g.ep_square.has_value());
        Assert::IsTrue(g.to_move == Color::White);
        Assert::AreEqual(g.fullmoves, (U16) 0);
        Assert::AreEqual(g.halfmove_clock, (U8) 0);
        Assert::AreEqual(g.castling_rights, (U8) 0);
    }

    TEST_METHOD(GAME_COMPARE)
    {
        Game g1;

        {
            Game g2;
            Assert::IsTrue(g1 == g2);
        }

        {
            Game g2;
            g2.board[11] = 0x2048;
            Assert::IsTrue(g1 != g2);
        }

        {
            Game g2;
            g2.ep_square = 11;
            Assert::IsTrue(g1 != g2);
        }

        {
            Game g2;
            g2.to_move = !g2.to_move;
            Assert::IsTrue(g1 != g2);
        }

        {
            Game g2;
            g2.fullmoves += 10;
            Assert::IsTrue(g1 != g2);
            g2.fullmoves -= 10;
            Assert::IsTrue(g1 == g2);
        }

        {
            Game g2;
            g2.halfmove_clock += 10;
            Assert::IsTrue(g1 != g2);
        }
    }

    TEST_METHOD(FROM_FEN)
    {
        { // starting position
            const auto og =
                game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            Assert::IsTrue(og.has_value());

            const Game g = *og;

            const auto og_nomoves =
                game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

            Assert::IsTrue(og_nomoves.has_value());

            const Game g_nomoves = *og_nomoves;

            Assert::IsTrue(g_nomoves == g);

            const auto og_only_halfmoves =
                game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0");

            Assert::IsFalse(og_only_halfmoves.has_value());

            using enum PieceType;

            Assert::AreEqual(get_pieces(g.board, Pawn, Color::White), BB("00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "11111111"
                                                                         "00000000"));

            Assert::AreEqual(get_pieces(g.board, Pawn, Color::Black), BB("00000000"
                                                                         "11111111"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"));

            Assert::AreEqual(get_pieces(g.board, Knight, Color::White), BB("00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "01000010"));

            Assert::AreEqual(get_pieces(g.board, Knight, Color::Black), BB("01000010"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"));

            Assert::AreEqual(get_pieces(g.board, Bishop, Color::White), BB("00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00100100"));

            Assert::AreEqual(get_pieces(g.board, Bishop, Color::Black), BB("00100100"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"
                                                                           "00000000"));

            Assert::AreEqual(get_pieces(g.board, Rook, Color::White), BB("00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "10000001"));

            Assert::AreEqual(get_pieces(g.board, Rook, Color::Black), BB("10000001"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"));

            Assert::AreEqual(get_pieces(g.board, Queen, Color::White), BB("00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00010000"));

            Assert::AreEqual(get_pieces(g.board, Queen, Color::Black), BB("00010000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"
                                                                          "00000000"));

            Assert::AreEqual(get_pieces(g.board, King, Color::White), BB("00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00001000"));

            Assert::AreEqual(get_pieces(g.board, King, Color::Black), BB("00001000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"
                                                                         "00000000"));

            Assert::IsFalse(g.ep_square.has_value());
            Assert::AreEqual(g.castling_rights,
                             static_cast<CastlingRights>(CASTLE_RIGHTS_WHITE_BOTH | CASTLE_RIGHTS_BLACK_BOTH));
            Assert::AreEqual(g.fullmoves, (U16) 1);
            Assert::AreEqual(g.halfmove_clock, (U8) 0);
        }

        { // invalid FEN strings
            Assert::IsFalse(game_from_fen("").has_value());
            Assert::IsFalse(game_from_fen("asdf").has_value());
        }
    }

    TEST_METHOD(TO_FEN)
    {
        for (std::string fen :
             {"r1bq1rk1/ppp1npbp/3p1np1/3Pp3/1PP1P3/2N2N2/P3BPPP/R1BQ1RK1 b - - 0 9",
              "rn1q1rk1/1pp2pp1/p3pb1p/8/P2Pb3/5NP1/1P2PPBP/RNQR2K1 b - - 1 14",
              "r1bq1rk1/pp3pbp/2n2np1/2p1p3/4PP2/2N2NP1/PPP3BP/R1B1QRK1 b - - 1 10",
              "rnbqkbnr/p1p2ppp/4p3/1pPp4/3P4/8/PP2PPPP/RNBQKBNR w KQkq b6 0 4"}) {

            const auto og = game_from_fen(fen);
            Assert::IsTrue(og.has_value());
            const Game& g = *og;

            Assert::AreEqual(fen, game_to_fen(g));
        }
    }
};
