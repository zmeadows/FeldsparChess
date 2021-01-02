export module game;

import prelude;
import bitboard;
import board;

#include "logger.h"
import<cstdio>;
import<cstring>;
import<cctype>;

export struct Game {
    Board board = {BITBOARD_EMPTY};
    MaybeSquare ep_square = {};
    Color to_move = Color::White;
    U16 fullmoves = 0;
    U8 halfmove_clock = 0;
    CastlingRights castling_rights = NO_CASTLING_RIGHTS;
};

export Optional<Game> build_game_from_fen_string(const char* fen)
{
    if (fen == nullptr) {
        WARN("Nullptr passed as FEN string!");
        return {};
    }

    constexpr auto MAX_FEN_LENGTH = 91;

    const auto N = strnlen(fen, MAX_FEN_LENGTH + 1);
    if (N > MAX_FEN_LENGTH) {
        WARN("FEN string too long!");
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

    size_t idx = 0;

    for (; idx <= MAX_FEN_LENGTH; idx++) {
        const char ch = fen[idx];

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
            decrement_square(static_cast<Square>(ch - '0'));
        }
        else if (ch == '/') {
            continue;
        }
        else {
            break;
        }
    }

    if (idx > MAX_FEN_LENGTH) {
        WARN("Invalid FEN string!");
        return {};
    }

    if (fen[idx] != ' ') {
        WARN("Invalid FEN string!");
        return {};
    }

    idx++;

    switch (fen[idx]) {
        case 'w': {
            game.to_move = Color::White;
            break;
        }
        case 'b': {
            game.to_move = Color::Black;
            break;
        }
        default: {
            WARN("Invalid FEN string!");
            return {};
        }
    }

    idx++;
    if (fen[idx] != ' ') {
        WARN("Invalid FEN string!");
        return {};
    }
    idx++;

    const size_t max_castle_idx = idx + 3;

    while (fen[idx] != ' ' && idx < max_castle_idx) {
        switch (fen[idx]) {
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
                WARN("Invalid FEN string!");
                return {};
            }
        }
    }

    return game;
}

// export inline const Game START_POSITION_GAME =
//     *build_game_from_fen_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
