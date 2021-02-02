import prelude;

import game;
import movegen;
import perft;
import print;
import init;
import attacks.base;

import unstd.io;

import<cstdlib>;
import<cstdio>;
import<string>;

int main()
{
    feldspar_init();

    // const Game og1 =
    //     *game_from_fen("rnbqkbnr/ppp1pppp/8/3p4/Q7/2P5/PP1PPPPP/RNB1KBNR b KQkq d6 1 2");
    // debug_print_movegen(og1);
    // TODO!!!! validate knight/pawn/king move tables

    // const Game og2 = *game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // perft_(og2, 5);

    // printf("\n\n\n");

    (void)qperft_validate<true>("rnbqkbnr/1ppppppp/8/p7/3P4/N7/PPP1PPPP/R1BQKBNR b KQkq - 0 2", 5);

    return EXIT_SUCCESS;
}
