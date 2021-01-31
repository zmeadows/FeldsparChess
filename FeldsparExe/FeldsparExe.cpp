import prelude;

import game;
import movegen;
import perft;
import print;

import unstd.io;
import unstd.string;

import<cstdlib>;
import<cstdio>;
import<string>;

int main()
{
    const Game og1 = *Game::create("rnbqkbnr/1ppppppp/8/p7/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 2");
    debug_print_movegen(og1);

    // const Game og2 = *Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // perft_(og2, 5);

    printf("\n\n\n");

    (void)qperft_validate("rnbqkbnr/1ppppppp/8/p7/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 2", 3);

    return EXIT_SUCCESS;
}
