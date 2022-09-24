#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import prelude;

import attacks;
import bitboard;
import game;
import init;
import perft;

import <iostream>;
import <string>;
import <vector>;

TEST_CLASS(PERFT_TEST) {

TEST_CLASS_INITIALIZE(FELDSPAR_INIT) {
    feldspar_init();
}

TEST_METHOD(CHESS_PROGRAMMING_WIKI_RESULTS)
{
    std::vector<PerftStats> results = perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);
    Assert::AreEqual(results[0].node_count, 20ULL);
    Assert::AreEqual(results[1].node_count, 400ULL);
    Assert::AreEqual(results[2].node_count, 8902ULL);
    Assert::AreEqual(results[3].node_count, 197281ULL);
    Assert::AreEqual(results[4].node_count, 4865609ULL);
    Assert::AreEqual(results[5].node_count, 119060324ULL);

    results = perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 5);
    Assert::AreEqual(results[0].node_count, 48ULL);
    Assert::AreEqual(results[1].node_count, 2039ULL);
    Assert::AreEqual(results[2].node_count, 97862ULL);
    Assert::AreEqual(results[3].node_count, 4085603ULL);
    Assert::AreEqual(results[4].node_count, 193690690ULL);

    results = perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 6);
    Assert::AreEqual(results[0].node_count, 14ULL);
    Assert::AreEqual(results[1].node_count, 191ULL);
    Assert::AreEqual(results[2].node_count, 2812ULL);
    Assert::AreEqual(results[3].node_count, 43238ULL);
    Assert::AreEqual(results[4].node_count, 674624ULL);
    Assert::AreEqual(results[5].node_count, 11030083ULL);

    results = perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);
    Assert::AreEqual(results[0].node_count, 6ULL);
    Assert::AreEqual(results[1].node_count, 264ULL);
    Assert::AreEqual(results[2].node_count, 9467ULL);
    Assert::AreEqual(results[3].node_count, 422333ULL);
    Assert::AreEqual(results[4].node_count, 15833292ULL);

    results = perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 5);
    Assert::AreEqual(results[0].node_count, 6ULL);
    Assert::AreEqual(results[1].node_count, 264ULL);
    Assert::AreEqual(results[2].node_count, 9467ULL);
    Assert::AreEqual(results[3].node_count, 422333ULL);
    Assert::AreEqual(results[4].node_count, 15833292ULL);

    results = perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5);
    Assert::AreEqual(results[0].node_count, 44ULL);
    Assert::AreEqual(results[1].node_count, 1486ULL);
    Assert::AreEqual(results[2].node_count, 62379ULL);
    Assert::AreEqual(results[3].node_count, 2103487ULL);
    Assert::AreEqual(results[4].node_count, 89941194ULL);
}

};

