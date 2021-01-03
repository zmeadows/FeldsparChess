#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import prelude;
import bitboard;
import board;
import game;

TEST(Game, Initialization)
{
    using enum PieceType;

    { // default 'empty' game state
        Game g;

        for (const PieceType p : EnumRange<PieceType>()) {
            for (const Color c : EnumRange<Color>()) {
                const Bitboard bb = get_pieces(g.board, p, c);
                EXPECT_EQ(bb, BITBOARD_EMPTY);
            }
        }

        EXPECT_FALSE(g.ep_square.has_value());
        EXPECT_EQ(g.to_move, Color::White);
        EXPECT_EQ(g.fullmoves, 0);
        EXPECT_EQ(g.halfmove_clock, 0);
        EXPECT_EQ(g.castling_rights, NO_CASTLING_RIGHTS);
    }

    { // starting position
        const Optional<Game> og =
            Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        ASSERT_TRUE(og.has_value());

        const Game g = *og;

        EXPECT_EQ(get_pieces(g.board, Pawn, Color::White), BB("00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "11111111"
                                                              "00000000"));

        EXPECT_EQ(get_pieces(g.board, Pawn, Color::Black), BB("00000000"
                                                              "11111111"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"));

        EXPECT_EQ(get_pieces(g.board, Knight, Color::White), BB("00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "01000010"));

        EXPECT_EQ(get_pieces(g.board, Knight, Color::Black), BB("01000010"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"));

        EXPECT_EQ(get_pieces(g.board, Bishop, Color::White), BB("00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00100100"));

        EXPECT_EQ(get_pieces(g.board, Bishop, Color::Black), BB("00100100"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"
                                                                "00000000"));

        EXPECT_EQ(get_pieces(g.board, Rook, Color::White), BB("00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "10000001"));

        EXPECT_EQ(get_pieces(g.board, Rook, Color::Black), BB("10000001"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"));

        EXPECT_EQ(get_pieces(g.board, Queen, Color::White), BB("00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00010000"));

        EXPECT_EQ(get_pieces(g.board, Queen, Color::Black), BB("00010000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"
                                                               "00000000"));

        EXPECT_EQ(get_pieces(g.board, King, Color::White), BB("00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00001000"));

        EXPECT_EQ(get_pieces(g.board, King, Color::Black), BB("00001000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"
                                                              "00000000"));

        EXPECT_FALSE(g.ep_square.has_value());
        EXPECT_EQ(g.castling_rights, FULL_CASTLING_RIGHTS);
        EXPECT_EQ(g.fullmoves, 1);
        EXPECT_EQ(g.halfmove_clock, 0);
    }

    { // invalid FEN strings
        EXPECT_FALSE(Game::create(nullptr).has_value());
        EXPECT_FALSE(Game::create("").has_value());
        EXPECT_FALSE(Game::create("asdf").has_value());
    }
}
