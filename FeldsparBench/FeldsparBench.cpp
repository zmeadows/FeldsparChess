import prelude;
import game;
import perft;
import init;

import<string>;
import<cstdio>;
#include <time.h>

double perft_benchmark(const std::string& name, U64 depth, U64 ntimes, const std::string& fen)
{
    const Game game = *game_from_fen(fen);

    double duration = 0.0;
    U64 node_count = 0;

    for (U64 i = 0; i < ntimes; i++) {
        const clock_t start = clock();
        const auto results = perft(game, depth);
        const clock_t stop = clock();

        for (const PerftStats& stats : results) {
            if (stats.node_count == 0) break;
            node_count += stats.node_count;
        }

        duration += static_cast<double>(stop - start) / CLOCKS_PER_SEC;
    }

    const double mnodes_sec = (static_cast<double>(node_count) / duration) / 1e6;

    printf("%s: %s\n", name.c_str(), fen.c_str());
    printf("\tNodes Traversed: %llu\n", node_count);
    printf("\tTotal Time: %f seconds\n", duration);
    printf("\tMNodes/sec: %f\n\n", mnodes_sec);

    return mnodes_sec;
}

int main()
{
    feldspar_init();

    double total = 0.0;

    total += perft_benchmark("Starting Position", 6, 5,
                             "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    total += perft_benchmark("Kiwipete", 5, 5,
                             "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");

    total += perft_benchmark("RookPawnEndGame", 7, 7, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");

    total += perft_benchmark("Position4", 6, 2,
                             "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    total += perft_benchmark("BugCatcher", 5, 5,
                             "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

    total +=
        perft_benchmark("StevenEdwards", 5, 5,
                        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

    printf("AVERAGE MNODES/SEC = %f\n", total / 6.0);

    return 0;
}
