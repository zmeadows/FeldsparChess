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
    const Game og1 = *Game::create("rnbqkbnr/ppppppp1/8/7p/7P/8/PPPPPPP1/RNBQKBNR w KQkq - 0 2");
    print_possible_moves(og1);

    const Game og2 = *Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    perft_(og2, 5);

    (void)qperft_divide("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4);
    (void)perft_divide("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4);

    return EXIT_SUCCESS;
}
