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
    //     *game_from_fen("rnb1kbnr/pp1p1pp1/7p/q1pPK3/8/7P/PPP1PPP1/RNBQ1BNR w kq - 0 8");
    // debug_print_movegen(og1);

    const Game og2 = *game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    perft_(og2, 6);

    // printf("\n\n\n");

    // (void)qperft_validate<true>("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5);

    return EXIT_SUCCESS;
}
