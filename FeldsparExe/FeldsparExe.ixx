import unstd.core;
import unstd.io;

import prelude;
import bitboard;
import attacks;
import game;
import perft;
import init;
import uci;

import <optional>;
import <string>;
import <vector>;


int main()
{
    feldspar_init();

    FeldsparUCI engine;

    return engine.run();
}
