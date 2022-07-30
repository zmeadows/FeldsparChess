export module uci;

import unstd.io;
import unstd.string_util;

import<string>;
import<iostream>;
import<unordered_map>;

export struct FeldsparUCI {
    int run(void)
    {
        std::string line, first_word;
        std::vector<std::string_view> words;

        while (!quitting && std::getline(std::cin, line)) {
            split(words, line);
            if (words.empty()) continue;
            first_word = words[0];
            std::invoke(DISPATCH_TABLE.at(first_word), *this, words);
        }

        return EXIT_SUCCESS;
    }

 private:
    void respond(std::string_view response) { to_stdout("{}\n", response); }

    using DispatchFn = void (FeldsparUCI::*)(const std::vector<std::string_view>& words);

    void respondto_uci(const std::vector<std::string_view>& words) { 
        respond("id name Feldspar");
        respond("id author Zac Meadows");
        respond("uciok");
    }

    void respondto_quit(const std::vector<std::string_view>& words) { 
        quitting = true;
    }

    void respondto_isready(const std::vector<std::string_view>& words) { 
        respond("readyok");
    }

    void respondto_setoption(const std::vector<std::string_view>& words) { 
    }

    void respondto_position(const std::vector<std::string_view>& words) { 
    }

    void respondto_go(const std::vector<std::string_view>& words) { 
    }

    void respondto_ucinewgame(const std::vector<std::string_view>& words) { 
    }

    static const inline std::unordered_map<std::string, DispatchFn> DISPATCH_TABLE = {
        {"uci", &FeldsparUCI::respondto_uci},
        {"quit", &FeldsparUCI::respondto_quit},
        {"isready", &FeldsparUCI::respondto_isready},
        {"setoption", &FeldsparUCI::respondto_setoption},
        {"position", &FeldsparUCI::respondto_position},
        {"go", &FeldsparUCI::respondto_go},
        {"ucinewgame", &FeldsparUCI::respondto_ucinewgame}
    };

    bool quitting = false;
};

