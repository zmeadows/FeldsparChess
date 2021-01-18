import game;
import move;
import movegen;
import zobrist;
import quad;
import bitboard;
import prelude;

import<cstdio>;
#include <cstddef>

int main()
{
    printf("%llu\n", alignof(QuadBitboard));
    printf("%llu\n", alignof(std::max_align_t));
    printf("%llu\n", sizeof(size_t));

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

    const QuadBitboard attacks = east_nort_noWe_noEa_Attacks(pack(rq, rq, bq, bq), empty);
    alignas(QuadBitboard) Bitboard bbs[4];
    unpack(attacks, bbs);

    const Bitboard attacked = reduceOR(attacks);
    print_bitboard(attacked);

    const Game og = *Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    size_t i = 0;
    MoveBuffer moves;
    while (i < 1e7) {
        generate_moves<false, false>(og, moves);
        i++;
    }

    printf("%llu\n", moves.length());
    printf("%llu\n", i);

    return 0;
}
