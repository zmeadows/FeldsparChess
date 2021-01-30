export module perft;

import unstd.core;
import unstd.io;
import unstd.string;

import game;
import move;
import movegen;

import<cstring>;
import<cstdio>;

// TODO: use my own hash map and string types
import<string>;
import<sstream>;
import<vector>;
import<map>;
import<iterator>;

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
        // TODO: add promotions, castles, etc...

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

export std::map<std::string, int> perft_divide(StringRef fen, U64 depth)
{
    if (depth == 0) return {};

    std::map<std::string, int> result;
    auto ogame = Game::create(fen);

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

    for (const auto& [m, c] : result) {
        printf("%s %d\n", m.c_str(), c);
    }

    return result;
}

template <typename Out>
void split(const std::string& s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        if (!item.empty()) {
            *result++ = item;
        }
    }
}

std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
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

    for (const auto& [m, c] : result) {
        printf("%s %d\n", m.c_str(), c);
    }

    return result;
}
