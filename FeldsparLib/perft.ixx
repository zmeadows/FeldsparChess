export module perft;

import prelude;
import unstd.io;
import unstd.string_util;

import game;
import move;
import movegen;

import <map>;
import <vector>;

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
        to_stdout("DEPTH {} NODES = {}\n", i + 1, stats[i].node_count);
    }
}

void perft_internal(Game& game, const U64 max_depth, U64 depth, std::vector<PerftStats>& stats)
{
    MoveBuffer moves;
    generate_moves(game, moves);

    if (depth == 1) {
        stats[max_depth - depth].node_count += moves.length();
        return;
    }

    const Game game_premove_copy(game);
    for (Move m : moves) {
        make_move<false>(game, m);
        // TODO: record promotions, castles, etc...
        stats[max_depth - depth].node_count++;
        perft_internal(game, max_depth, depth - 1, stats);
        memcpy(&game, &game_premove_copy, sizeof(Game));
    }
}

export std::vector<PerftStats> perft(const Game& game, U64 depth)
{
    std::vector<PerftStats> stats(depth);
    Game game_copy = game;
    perft_internal(game_copy, depth, depth, stats);
    return stats;
}

export std::vector<PerftStats> perft(const std::string& fen, U64 depth)
{
    to_stdout(fen);
    const Game game = *game_from_fen(fen);
    return perft(game, depth);
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

    //assert(ogame.has_value());

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
        } else if (monitor_lines &&
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

template <bool DEBUG_PRINT = true>
void qperft_validate_internal(const std::string& fen, U64 max_depth, bool& failed)
{
    if (failed) return;

    for (U64 depth = 3; depth <= max_depth; depth++) {
        const auto qperft_results = qperft_divide(fen.c_str(), depth);
        const auto feldspar_results = perft_divide(fen, depth);

        std::vector<std::string> missing_moves;
        for (const auto& [move_alg, _] : qperft_results) {
            if (auto it = feldspar_results.find(move_alg); it == feldspar_results.end()) {
                missing_moves.push_back(move_alg);
            }
        }

        std::vector<std::string> illegal_moves;
        for (const auto& [move_alg, _] : feldspar_results) {
            if (auto it = qperft_results.find(move_alg); it == qperft_results.end()) {
                illegal_moves.push_back(move_alg);
            }
        }

        failed = missing_moves.size() > 0 || illegal_moves.size() > 0;

        if (failed) {
            if constexpr (DEBUG_PRINT) {
                to_stdout("{}\n", fen);
                to_stdout("Feldspar failed to generate the following legal moves:\n");
                for (const std::string m : missing_moves) {
                    to_stdout("\t{}\n", m);
                }
                to_stdout("Feldspar generate the following non-legal moves:\n");
                for (const std::string m : illegal_moves) {
                    to_stdout("\t{}\n", m);
                }
            }

            return;
        }

        Game game = *game_from_fen(fen);
        MoveBuffer moves;
        generate_moves(game, moves);

        const Game game_premove_copy(game);

        for (const auto& [move_alg, qperft_count] : qperft_results) {
            auto it = feldspar_results.find(move_alg);
            assert(it != feldspar_results.end());
            if (it->second != qperft_count) {
                const auto m = *move_from_algebraic(moves, move_alg);
                make_move<false>(game, m);
                qperft_validate_internal<DEBUG_PRINT>(game_to_fen(game), max_depth - 1, failed);
                if (failed) return;
                memcpy(&game, &game_premove_copy, sizeof(Game));
            }
        }
    }
}

export template <bool DEBUG_PRINT = true>
bool qperft_validate(const std::string& fen, U64 max_depth)
{
    bool failed = false;
    qperft_validate_internal<DEBUG_PRINT>(fen, max_depth, failed);
    return failed;
}
