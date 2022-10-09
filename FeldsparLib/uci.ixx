export module uci;

import unstd.core;
import unstd.io;
import unstd.string_util;
import unstd.random;

import prelude;
import game;
import move;
import movegen;

import <chrono>;
import <iostream>;
import <optional>;
import <string>;
import <thread>;
import <unordered_map>;
import <vector>;

export struct EngineUCI {
    int run(void)
    {
        std::string line, first_word;
        line.reserve(2048);
        std::vector<std::string> words;

        while (!_quitting && std::getline(std::cin, line)) {
            words = split(line);
            if (words.empty()) continue;
            first_word = words[0];
            std::invoke(DISPATCH_TABLE.at(first_word), *this, words);
        }

        return 0;
    }

 private:
    template <class... Args>
    void respond(std::string_view fmt, Args&&... args)
    {
        std::cout << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
    }

    using DispatchFn = void (EngineUCI::*)(const std::vector<std::string>& words);

    void respondto_uci(const std::vector<std::string>& words)
    { 
        respond("id name Feldspar");
        respond("id author Zac Meadows");
        respond("uciok");
    }

    void respondto_quit(const std::vector<std::string>& words) { 
        _quitting = true;
    }

    void respondto_isready(const std::vector<std::string>& words) { 
        respond("readyok");
    }

    void respondto_setoption(const std::vector<std::string>& words) { 
    }

    void respondto_position(const std::vector<std::string>& words)
    {
        assert(words.size() >= 2);

        // TODO: optimize for incremental updates
        // if (!_game.has_value()) {
        //     _parse_new_game(words);
        // } else {
        //     _update_game(words[words.size() - 1]);
        // }

        std::optional<size_t> moves_start_idx = {};

        if (words[1] == "startpos") {
            _game = starting_position();
            if (words.size() > 3 && words[2] == "moves") {
                moves_start_idx = 3;
            }
        } else {
            std::string fen_string;
            for (size_t i = 2; i < words.size(); i++) {
                if (words[i] == "moves") {
                    moves_start_idx = i + 1;
                    break;
                }
                fen_string += words[i];
                fen_string.push_back(' ');
            }
            fen_string.pop_back(); // remove the last space character

            if (std::optional<Game> og = game_from_fen(fen_string); og.has_value()) {
                _game = *og;
            } else {
                std::cerr << "Failed to parse FEN string: " << fen_string << std::endl;
            }
        }

        if (moves_start_idx.has_value()) {
            bool success = make_moves_algebraic(*_game, words.begin() + *moves_start_idx, words.end());
            if (!success) {
                std::cerr << "Failed to play moves!" << std::endl;
            }
        }
    }

    void respondto_go(const std::vector<std::string>& words) { 
        assert(_game.has_value());
        MoveBuffer moves;
        generate_moves(*_game, moves);
        assert(moves.length() > 0);

        Move chosen_move = moves[rand<U64>() % moves.length()];
        respond("bestmove {}", move_to_algebraic(chosen_move));
    }

    void respondto_ucinewgame(const std::vector<std::string>& words) { 
        _game.reset();
    }

    static const inline std::unordered_map<std::string, DispatchFn> DISPATCH_TABLE = {
        {"uci", &EngineUCI::respondto_uci},
        {"quit", &EngineUCI::respondto_quit},
        {"isready", &EngineUCI::respondto_isready},
        {"setoption", &EngineUCI::respondto_setoption},
        {"position", &EngineUCI::respondto_position},
        {"go", &EngineUCI::respondto_go},
        {"ucinewgame", &EngineUCI::respondto_ucinewgame}
    };

    bool _quitting = false;
    std::optional<Game> _game = {};
};


