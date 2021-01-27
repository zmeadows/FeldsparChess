export module perft;

import unstd.core;
import unstd.array;

import game;
import move;
import movegen;

import<cstring>;
import<cstdio>;

export struct PerftStats {
    U64 node_count = 0;
    U64 captures = 0;
    U64 ep_captures = 0;
    U64 castles = 0;
    U64 promotions = 0;
    U64 checks = 0;
    U64 check_mates = 0;
};

export template <U64 MAX_DEPTH>
void print_perft_stats(const Array<PerftStats, MAX_DEPTH>& stats)
{
    for (U64 i = 0; i < MAX_DEPTH; i++) {
        printf("DEPTH %llu NODES = %llu\n", i, stats[i].node_count);
    }
}

template <U64 MAX_DEPTH>
void perft_internal(Game& game, U64 depth, Array<PerftStats, MAX_DEPTH>& stats)
{
    if (depth == 0) return;

    MoveBuffer moves;
    generate_moves(game, moves);

    const Game game_premove_copy(game);
    for (Move m : moves) {
        make_move(game, m);

        stats[MAX_DEPTH - depth].node_count++;
        // TODO: add promotions, castles, etc...

        perft_internal(game, depth - 1, stats);
        memcpy(&game, &game_premove_copy, sizeof(Game));
    }
}

export template <U64 MAX_DEPTH>
Array<PerftStats, MAX_DEPTH> perft(const Game& game)
{
    Array<PerftStats, MAX_DEPTH> stats;
    Game game_copy = game;
    perft_internal(game_copy, MAX_DEPTH, stats);
    return stats;
}

export template <U64 MAX_DEPTH>
void perft_(const Game& game)
{
    const auto results = perft<MAX_DEPTH>(game);
    print_perft_stats(results);
}
