export module perft;

import unstd.core;
import unstd.io;
import unstd.string_util;

import game;
import move;
import movegen;

import<cstring>;
import<cstdio>;

import<string>;
import<vector>;
import<map>;

export struct PerftStats {
    U64 node_count = 0;
    U64 captures = 0;
    U64 ep_captures = 0;
    U64 castles = 0;
    U64 promotions = 0;
    U64 checks = 0;
    U64 check_mates = 0;
};

void print_perft_stats(const std::vector<PerftStats>& stats)
{
    for (U64 i = 0; i < stats.size(); i++) {
        printf("DEPTH %llu NODES = %llu\n", i, stats[i].node_count);
    }
}

void perft_internal(Game& game, const U64 max_depth, U64 depth, std::vector<PerftStats>& stats)
{
    if (depth == 0) return;

    MoveBuffer moves;
    generate_moves(game, moves);

    const Game game_premove_copy(game);
    for (Move m : moves) {
        make_move<false>(game, m);

        stats[max_depth - depth].node_count++;
        // TODO: record promotions, castles, etc...

        perft_internal(game, max_depth, depth - 1, stats);
        memcpy(&game, &game_premove_copy, sizeof(Game));
    }
}

std::vector<PerftStats> perft(const Game& game, U64 depth)
{
    std::vector<PerftStats> stats(depth);
    Game game_copy = game;
    perft_internal(game_copy, depth, depth, stats);
    return stats;
}

export void perft_(const Game& game, U64 depth)
{
    const auto results = perft(game, depth);
    print_perft_stats(results);
}

export std::map<std::string, int> perft_divide(const std::string& fen, U64 depth)
{
    if (depth == 0) return {};

    std::map<std::string, int> result;
    auto ogame = game_from_fen(fen);

    if (!ogame.has_value()) return {};

    Game game = *ogame;

    MoveBuffer moves;
    generate_moves(game, moves);
    const Game game_premove_copy(game);
    for (Move m : moves) {
        make_move<false>(game, m);
        const auto stats = perft(game, depth - 1);
        result[move_to_algebraic(m)] = (int)stats.back().node_count;
        memcpy(&game, &game_premove_copy, sizeof(Game));
    }

    return result;
}

export std::map<std::string, int> qperft_divide(const char* fen, U64 depth)
{
    // TODO: use SOLUTION_DIR macro
    std::string qperft_cmd(SOLUTION_DIR);
    qperft_cmd += "Resources\\qperft.exe";
    qperft_cmd += " ";
    qperft_cmd += std::to_string(depth);
    qperft_cmd += " H20";
    qperft_cmd += " -";
    qperft_cmd += std::to_string(depth - 1);
    qperft_cmd += " \"";
    qperft_cmd += std::string(fen);
    qperft_cmd += "\"";

    bool monitor_lines = false;
    const std::string depth_marker = "perft( " + std::to_string(depth - 1) + ")=";
    const std::string over_depth_marker = "perft( " + std::to_string(depth) + ")=";

    std::map<std::string, int> result;

    for_each_line_in_process_stdout(qperft_cmd.c_str(), [&](const std::string& str) {
        if (!monitor_lines && str.compare(0, depth_marker.size(), depth_marker) == 0) {
            monitor_lines = true;
            return true;
        }
        else if (monitor_lines &&
                 str.compare(0, over_depth_marker.size(), over_depth_marker) == 0) {
            monitor_lines = false;
            return false;
        }

        if (monitor_lines && str[0] == '2' && str[1] == '.') {
            const auto words = split(str, ' ');
            result[words[1]] = std::stoi(words[4]);
        }

        return true;
    });

    return result;
}

export bool qperft_validate(const char* const fen, U64 max_depth)
{
    for (U64 depth = 3; depth <= max_depth; depth++) {
        const auto qperft_results = qperft_divide(fen, depth);
        const auto feldspar_results = perft_divide(fen, depth);

        for (const auto& [move_alg, count] : qperft_results) {
            int feldspar_count = 0;
            if (auto it = feldspar_results.find(move_alg); it != feldspar_results.end()) {
                feldspar_count = it->second;
            }
            if (count != feldspar_count) {
                printf("%s %d %d\n", move_alg.c_str(), count, feldspar_count);
            }
        }
    }
    return true;
}
