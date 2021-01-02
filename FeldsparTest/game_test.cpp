#define _HAS_EXCEPTIONS 0
#include "gtest/gtest.h"

import prelude;
import bitboard;
import board;
import game;
TEST(Game, Initialization)
{
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
    }
}
