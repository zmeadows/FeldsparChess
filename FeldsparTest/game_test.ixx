#include "gtest/gtest.h"

import prelude;
import bitboard;
import board;
import game;

TEST(Game, Initialization)
{
    // Game default_game();

    for (const PieceType ptype : EnumRange<PieceType>()) {
        EXPECT_EQ(ptype, ptype);
    }
}
