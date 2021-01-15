import game;
import move;
import movegen;
import zobrist;
import quad;
import bitboard;
import prelude;

import<cstdio>;

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

    const QuadBitboard attacks = east_nort_noWe_noEa_Attacks(pack(rq, rq, bq, bq), empty);
    Bitboard bbs[4];
    unpack(attacks, bbs);

    const Bitboard attacked = bbs[0] | bbs[1] | bbs[2] | bbs[3];
    print_bitboard(attacked);

    init_zobrist_hashing();

    const Game og = *Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    MoveBuffer moves;
    while (true) {
        generate_moves<false, false>(og, moves);
    }

    printf("%llu\n", moves.length());

    return 0;
}
