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
import<cassert>;

int main()
{
    feldspar_init();

    const Game og1 =
        *game_from_fen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    perft_(og1, 8);

    return 0;
}
