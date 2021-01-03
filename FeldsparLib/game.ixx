export module game;

import prelude;
import bitboard;
import board;

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
    MaybeSquare ep_square = {};
    Color to_move = Color::White;
    U16 fullmoves = 0;
    U8 halfmove_clock = 0;
    CastlingRights castling_rights = NO_CASTLING_RIGHTS;

    static Optional<Game> create(const char*);
};

// https://www.bfilipek.com/2018/07/string-view-perf-followup.html
std::vector<std::string_view> split_string(std::string_view strv, std::string_view delims = " ")
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
}

Optional<int> string_view_to_int(std::string_view sv)
{
    int number;
    auto result = std::from_chars(sv.data(), sv.data() + sv.size(), number);
    if (result.ec == std::errc()) {
        return number;
    }
    else {
        return {};
    }
}

Optional<Game> create_game_internal(std::string_view fen)
{
    if (fen.size() == 0) {
        return {};
    }

    const auto fen_pieces = split_string(fen);
    if (fen_pieces.size() != 6) {
        WARN("Invalid FEN string with number of whitespaces != 6");
        return {};
    }

    Game game;

    Square current_square = 63;
    constexpr auto decrement_square = [&](Square n) -> void {
        if (current_square >= n) {
            current_square -= n;
        }
        else {
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
        else {
            WARN("Invalid character encountered in FEN board representation.");
            return {};
        }
    }

    auto fen_color = fen_pieces[1];

    if (fen_color.size() != 1) {
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
        default: {
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
                default: {
                    WARN("Invalid FEN castling rights string (should be combination of K,Q,k,q "
                         "characters).");
                    return {};
                }
            }
        }
    }

    auto fen_ep = fen_pieces[3];

    if (fen_ep != "-") {
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

    auto fen_halfmove = fen_pieces[4];
    if (auto count = string_view_to_int(fen_halfmove); count.has_value()) {
        game.halfmove_clock = *count;
    }

    auto fen_fullmoves = fen_pieces[5];
    if (auto count = string_view_to_int(fen_fullmoves); count.has_value()) {
        game.fullmoves = *count;
    }

    return game;
}

export Optional<Game> Game::create(const char* fen)
{
    if (fen == nullptr) {
        WARN("Passed nullptr to Game::create.");
        return {};
    }
    else if (strnlen(fen, 100) == 100) {
        WARN("Passed invalid FEN string that was too long to Game::create.");
        return {};
    }
    else {
        return create_game_internal(fen);
    }
}

// export inline const Game START_POSITION_GAME =
//     *build_game_from_fen_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
