#include "gtest/gtest.h"
#include "init_fixture.h"

import prelude;
import bitboard;
import game;
import perft;
import attacks.classical;

class PerftTest : public InitFixture {};

TEST_METHOD(PerftTest, ChessProgrammingResults)
{
    {
        const auto results = perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);
        Assert::AreEqual(results[0].node_count, 20);
        Assert::AreEqual(results[1].node_count, 400);
        Assert::AreEqual(results[2].node_count, 8902);
        Assert::AreEqual(results[3].node_count, 197281);
        Assert::AreEqual(results[4].node_count, 4865609);
        Assert::AreEqual(results[5].node_count, 119060324);
    }

    {
        const auto results =
            perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 5);
        Assert::AreEqual(results[0].node_count, 48);
        Assert::AreEqual(results[1].node_count, 2039);
        Assert::AreEqual(results[2].node_count, 97862);
        Assert::AreEqual(results[3].node_count, 4085603);
        Assert::AreEqual(results[4].node_count, 193690690);
    }

    {
        const auto results = perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 6);
        Assert::AreEqual(results[0].node_count, 14);
        Assert::AreEqual(results[1].node_count, 191);
        Assert::AreEqual(results[2].node_count, 2812);
        Assert::AreEqual(results[3].node_count, 43238);
        Assert::AreEqual(results[4].node_count, 674624);
        Assert::AreEqual(results[5].node_count, 11030083);
    }

    {
        const auto results =
            perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);
        Assert::AreEqual(results[0].node_count, 6);
        Assert::AreEqual(results[1].node_count, 264);
        Assert::AreEqual(results[2].node_count, 9467);
        Assert::AreEqual(results[3].node_count, 422333);
        Assert::AreEqual(results[4].node_count, 15833292);
    }

    {
        const auto results =
            perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 5);
        Assert::AreEqual(results[0].node_count, 6);
        Assert::AreEqual(results[1].node_count, 264);
        Assert::AreEqual(results[2].node_count, 9467);
        Assert::AreEqual(results[3].node_count, 422333);
        Assert::AreEqual(results[4].node_count, 15833292);
    }

    {
        const auto results = perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5);
        Assert::AreEqual(results[0].node_count, 44);
        Assert::AreEqual(results[1].node_count, 1486);
        Assert::AreEqual(results[2].node_count, 62379);
        Assert::AreEqual(results[3].node_count, 2103487);
        Assert::AreEqual(results[4].node_count, 89941194);
    }
}
