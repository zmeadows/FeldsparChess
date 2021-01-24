export module game;

import prelude;
import bitboard;
import board;
import zobrist;

#include "logger.h"
import<cstdio>;
import<cstring>;
import<cctype>;

import<string_view>;
import<vector>;
import<charconv>;
import<system_error>;

export struct Game {
    Board board = {BITBOARD_EMPTY};
    GameHash hash = 0;
    MaybeSquare ep_square = {};
    Color to_move = Color::White;
    U16 fullmoves = 0;
    U8 halfmove_clock = 0;
    CastlingRights castling_rights = NO_CASTLING_RIGHTS;

    bool operator==(const Game&) const = default;

    static Optional<Game> create(const char*);
};

export GameHash create_game_hash(const Game& game)
{
    GameHash hash = 0;

    for (Color color : EnumRange<Color>()) {
    }

    return hash;
}

// TODO: switch to using basic const char* and C functions
Optional<Game> create_game_internal(std::string_view fen)
{
    if (fen.size() == 0) [[unlikely]] {
        return {};
    }

    // clang-format off
    // https://www.bfilipek.com/2018/07/string-view-perf-followup.html
    constexpr auto split_string = [](
        std::string_view strv,
        std::string_view delims = " "
        ) -> std::vector<std::string_view>
    {
        std::vector<std::string_view> output;
        size_t first = 0;

        while (first < strv.size()) {
            const auto second = strv.find_first_of(delims, first);

            if (first != second) output.emplace_back(strv.substr(first, second - first));

            if (second == std::string_view::npos) break;

            first = second + 1;
        }

        return output;
    };
    // clang-format on

    const auto fen_pieces = split_string(fen);
    // We consider a FEN string valid even if it doesn't specify half and/or full move counts,
    // in which case they are set to 0 and 1 respectively, as at the start of a game.
    if (fen_pieces.size() != 4 && fen_pieces.size() != 6) [[unlikely]] {
        WARN("Invalid FEN string with wrong number of whitespaces");
        return {};
    }

    Game game;

    Square current_square = 63;
    constexpr auto decrement_square = [&](Square n) -> void {
        if (current_square >= n) [[likely]] {
            current_square -= n;
        }
        else [[unlikely]] {
            current_square = 0;
        }
    };

    constexpr auto add_piece = [&](PieceType ptype, Color col) {
        const Bitboard bitrep = square_bitrep(current_square);
        get_pieces_mut(game.board, ptype, col) |= bitrep;
        get_occupied_mut(game.board, col) |= bitrep;
        decrement_square(1);
    };

    using enum PieceType;

    // Piece Locations
    for (const char ch : fen_pieces[0]) {
        if (isalpha(ch)) {
            const Color col = isupper(ch) ? Color::White : Color::Black;
            switch (tolower(ch)) {
                case 'p': {
                    add_piece(Pawn, col);
                    break;
                }
                case 'n': {
                    add_piece(Knight, col);
                    break;
                }
                case 'b': {
                    add_piece(Bishop, col);
                    break;
                }
                case 'r': {
                    add_piece(Rook, col);
                    break;
                }
                case 'q': {
                    add_piece(Queen, col);
                    break;
                }
                case 'k': {
                    add_piece(King, col);
                    break;
                }
            }
        }
        else if (isdigit(ch)) {
            decrement_square(static_cast<Square>(ch) - static_cast<Square>('0'));
        }
        else if (ch == '/') {
            continue;
        }
        else [[unlikely]] {
            WARN("Invalid character encountered in FEN board representation.");
            return {};
        }
    }

    auto fen_color = fen_pieces[1];

    if (fen_color.size() != 1) [[unlikely]] {
        WARN("Invalid FEN color string length (should be 1).");
        return {};
    }

    switch (fen_color[0]) {
        case 'w': {
            game.to_move = Color::White;
            break;
        }
        case 'b': {
            game.to_move = Color::Black;
            break;
        }
        default:
            [[unlikely]]
            {
                WARN("Invalid FEN color string value (should be w or b).");
                return {};
            }
    }

    auto fen_castle = fen_pieces[2];

    if (fen_castle == "-") {
        game.castling_rights = NO_CASTLING_RIGHTS;
    }
    else {
        for (const char ch : fen_castle) {
            switch (ch) {
                case 'K': {
                    game.castling_rights |= WHITE_KINGSIDE;
                    break;
                }
                case 'Q': {
                    game.castling_rights |= WHITE_QUEENSIDE;
                    break;
                }
                case 'k': {
                    game.castling_rights |= BLACK_KINGSIDE;
                    break;
                }
                case 'q': {
                    game.castling_rights |= BLACK_QUEENSIDE;
                    break;
                }
                default:
                    [[unlikely]]
                    {
                        WARN("Invalid FEN castling rights string (should be combination of K,Q,k,q "
                             "characters).");
                        return {};
                    }
            }
        }
    }

    auto fen_ep = fen_pieces[3];

    if (fen_ep != "-") [[unlikely]] {
        if (fen_ep.size() != 2) {
            WARN("Invalid FEN en passant target square string.");
            return {};
        }

        if (MaybeSquare sq = from_algebraic(fen_ep[0], fen_ep[1]); sq.has_value()) {
            game.ep_square = sq;
        }
        else {
            WARN("Couldn't convert En Passant target square from algebraic notation.");
            return {};
        }
    }

    if (fen_pieces.size() >= 5) [[likely]] {
        auto fen_halfmove = fen_pieces[4];
        game.halfmove_clock = atoi(std::string(fen_halfmove).c_str());
    }
    else [[unlikely]] {
        game.halfmove_clock = 0;
    }

    if (fen_pieces.size() >= 6) [[likely]] {
        auto fen_fullmoves = fen_pieces[5];
        game.fullmoves = atoi(std::string(fen_fullmoves).c_str());
    }
    else [[unlikely]] {
        game.fullmoves = 1;
    }

    game.hash = create_game_hash(game);

    return game;
}

export Optional<Game> Game::create(const char* fen)
{
    if (fen == nullptr) [[unlikely]] {
        WARN("Passed nullptr to Game::create.");
        return {};
    }
    else if (strnlen(fen, 100) == 100) [[unlikely]] {
        WARN("Passed invalid FEN string that was too long to Game::create.");
        return {};
    }
    else [[likely]] {
        return create_game_internal(fen);
    }
}

// tactical positions
// 3q1r1k/2r2ppp/2p1b3/1p2P2R/p1pP2Nb/P3Q2P/1P2B1P1/5RK1 w - - 5 27 (N -> F6)

// export inline const Game START_POSITION_GAME =
//     *build_game_from_fen_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
