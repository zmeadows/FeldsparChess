import prelude;

import game;
import movegen;
import perft;
import print;
import init;

import unstd.io;

import<cstdlib>;
import<cstdio>;
import<string>;

int main()
{
    feldspar_init();

    // const Game og1 = *game_from_fen("rnbqkbnr/1ppppppp/8/p7/4P3/8/PPPPQPPP/RNB1KBNR b KQkq a6 1
    // 2"); debug_print_movegen(og1);

    // const Game og2 = *game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // perft_(og2, 5);

    printf("\n\n\n");

    (void)qperft_validate<true>("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1", 5);

    return EXIT_SUCCESS;
}
