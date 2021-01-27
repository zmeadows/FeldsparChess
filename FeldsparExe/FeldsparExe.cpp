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
#include <cstdint>
#include <cstdio>;

int main()
{
    const Bitboard rq = BB("00000000"
                           "00000000"
                           "00000000"
                           "00000000"
                           "00000000"
                           "00000010"
                           "00001000"
                           "00010000");

    const Bitboard bq = BB("00000000"
                           "00000000"
                           "00000000"
                           "00000000"
                           "00000000"
                           "00100000"
                           "00001000"
                           "00000001");

    const Bitboard empty = BB("00000000"
                              "00000000"
                              "00000000"
                              "11111111"
                              "11111111"
                              "11111111"
                              "11111111"
                              "11111111");

    print_bitboard(get_knight_moves(f6));
    printf("%llu\n", set_bits<h6, f6>());

    const Game og1 = *Game::create("rnbqkbnr/ppppppp1/8/7p/7P/8/PPPPPPP1/RNBQKBNR w KQkq - 0 2");
    print_possible_moves(og1);

    const Game og = *Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    perft_<3>(og);

    return 0;
}
