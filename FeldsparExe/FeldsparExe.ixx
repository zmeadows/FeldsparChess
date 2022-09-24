import prelude;

import unstd.core;
import unstd.io;

import attacks;
import bitboard;
import game;
import init;
import move;
import movegen;
import perft;
import uci;

import <iostream>;
import <optional>;
import <string>;
import <vector>;

int main()
{
    feldspar_init();

    // Game g = *game_from_fen("1nbk1b2/r2p1pQp/p6P/nBp3p1/P3P3/8/1RpP1P2/2BNK3 b - - 2 29");

    // MoveBuffer moves;
    // generate_moves<{.DEBUG_PRINT = true}>(g, moves);

    // for (Move m : moves)
    //     std::cout << move_to_algebraic(m) << std::endl;

    EngineUCI engine;
    return engine.run();
}
