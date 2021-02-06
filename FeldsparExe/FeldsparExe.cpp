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

    const Game game = *game_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    const auto results = perft(game, 4);

    return 0;
}
