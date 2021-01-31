export module game;

import prelude;
import bitboard;
import board;
import zobrist;

import unstd.string_util;

import<cassert>;
import<cstdio>;
import<cctype>;
import<cstdlib>;
import<cstring>;

import<string>;
import<vector>;
#include "logger.h"

export struct Game {
    Board board = {BITBOARD_EMPTY};
    GameHash hash = 0;
    MaybeSquare ep_square = {};
    Color to_move = Color::White;
    U16 fullmoves = 0;
    U8 halfmove_clock = 0;
    CastlingRights castling_rights = NO_CASTLING_RIGHTS;

    bool operator==(const Game&) const = default;
};

export GameHash create_game_hash(const Game& game)
{
    GameHash hash = 0;

    for (Color color : EnumRange<Color>()) {
    }

    return hash;
}

// TODO: implement
MaybeSquare from_algebraic(std::string alg)
{
    assert(alg.length() == 2);
    return {};
}

Optional<Game> game_from_fen_internal(const std::string& fen)
{
    if (fen.length() == 0) [[unlikely]] {
        return {};
    }

    const std::vector<std::string> fen_pieces = split(fen, ' ');
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

    if (fen_color.length() != 1) [[unlikely]] {
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
        if (fen_ep.length() != 2) {
            WARN("Invalid FEN en passant target square string.");
            return {};
        }

        if (MaybeSquare sq = from_algebraic(fen_ep); sq.has_value()) {
            game.ep_square = sq;
        }
        else {
            WARN("Couldn't convert En Passant target square from algebraic notation.");
            return {};
        }
    }

    if (fen_pieces.size() >= 5) [[likely]] {
        game.halfmove_clock = atoi(fen_pieces[4].c_str());
    }
    else [[unlikely]] {
        game.halfmove_clock = 0;
    }

    if (fen_pieces.size() >= 6) [[likely]] {
        game.fullmoves = atoi(fen_pieces[5].c_str());
    }
    else [[unlikely]] {
        game.fullmoves = 1;
    }

    game.hash = create_game_hash(game);

    return game;
}

export Optional<Game> game_from_fen(const std::string& fen)
{
    if (fen.length() == 0) [[unlikely]] {
        WARN("Passed empty FEN string to Game::create.");
        return {};
    }
    else if (fen.length() > 92) [[unlikely]] {
        WARN("Passed invalid FEN string that was too long to Game::create.");
        return {};
    }
    else [[likely]] {
        return game_from_fen_internal(fen);
    }
}

export std::string game_to_fen(const Game& game) { return ""; }

// tactical positions
// 3q1r1k/2r2ppp/2p1b3/1p2P2R/p1pP2Nb/P3Q2P/1P2B1P1/5RK1 w - - 5 27 (N -> F6)

// export inline const Game START_POSITION_GAME =
//     *build_game_from_fen_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
