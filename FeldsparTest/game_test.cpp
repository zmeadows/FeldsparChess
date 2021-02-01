#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import prelude;
import bitboard;
import board;
import game;

TEST(Game, Initialization)
{

    // default 'empty' game state
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

TEST(Game, ComparisonOperator)
{
    Game g1;

    {
        Game g2;
        EXPECT_EQ(g1, g2);
    }

    {
        Game g2;
        g2.board[11] = 0x2048;
        EXPECT_NE(g1, g2);
    }

    {
        Game g2;
        g2.ep_square = 11;
        EXPECT_NE(g1, g2);
    }

    {
        Game g2;
        g2.to_move = !g2.to_move;
        EXPECT_NE(g1, g2);
    }

    {
        Game g2;
        g2.fullmoves += 10;
        EXPECT_NE(g1, g2);
        g2.fullmoves -= 10;
        EXPECT_EQ(g1, g2);
    }

    {
        Game g2;
        g2.halfmove_clock += 10;
        EXPECT_NE(g1, g2);
    }
}

TEST(Game, GameFromFEN)
{
    { // starting position
        const Optional<Game> og =
            game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        ASSERT_TRUE(og.has_value());

        const Game g = *og;

        const Optional<Game> og_nomoves =
            game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

        ASSERT_TRUE(og_nomoves.has_value());

        const Game g_nomoves = *og_nomoves;

        EXPECT_EQ(g_nomoves, g);

        const Optional<Game> og_only_halfmoves =
            game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0");

        EXPECT_FALSE(og_only_halfmoves.has_value());

        using enum PieceType;

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
        EXPECT_FALSE(game_from_fen("").has_value());
        EXPECT_FALSE(game_from_fen("asdf").has_value());
    }
}

TEST(Game, GameFromToFEN)
{
    for (std::string fen : {"r1bq1rk1/ppp1npbp/3p1np1/3Pp3/1PP1P3/2N2N2/P3BPPP/R1BQ1RK1 b - - 0 9",
                            "rn1q1rk1/1pp2pp1/p3pb1p/8/P2Pb3/5NP1/1P2PPBP/RNQR2K1 b - - 1 14",
                            "r1bq1rk1/pp3pbp/2n2np1/2p1p3/4PP2/2N2NP1/PPP3BP/R1B1QRK1 b - - 1 10",
                            "rnbqkbnr/p1p2ppp/4p3/1pPp4/3P4/8/PP2PPPP/RNBQKBNR w KQkq b6 0 4"}) {

        const auto og = game_from_fen(fen);
        ASSERT_TRUE(og.has_value());
        const Game& g = *og;

        EXPECT_EQ(fen, game_to_fen(g));
    }
}
