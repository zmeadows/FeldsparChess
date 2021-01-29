import prelude;

import game;
import move;
import movegen;
import zobrist;
import quad;
import bitboard;
import perft;
import print;
import tables;

#include <cstddef>
import<cstdint>;
import<cstdio>;

int main()
{
    const Game og1 = *Game::create("rnbqkbnr/ppppppp1/8/7p/7P/8/PPPPPPP1/RNBQKBNR w KQkq - 0 2");
    print_possible_moves(og1);

    const Game og = *Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    perft_<6>(og);

    return 0;
}
