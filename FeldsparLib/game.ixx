export module game;

#include "unstd/macros.h"
import prelude;
import unstd.io;
import unstd.string_util;

import bitboard;
import board;
import zobrist;

import <optional>;
import <string>;
import <vector>;

export struct Game {
    Board board = {BITBOARD_EMPTY};
    GameHash hash = 0;
    MaybeSquare ep_square = {};
    Color to_move = Color::White;
    U16 fullmoves = 0;
    U8 halfmove_clock = 0;
    CastlingRights castling_rights = 0;

    bool operator==(const Game&) const = default;
};

export GameHash create_game_hash(const Game& game)
{
    GameHash hash = 0;

    for (Color color : EnumRange<Color>()) {
    }

    return hash;
}

std::optional<Game> game_from_fen_internal(const std::string& fen)
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
    auto decrement_square = [&](Square n) -> void {
        if (current_square >= n) [[likely]] {
            current_square -= n;
        } else [[unlikely]] {
            current_square = 0;
        }
    };

    auto add_piece = [&](PieceType ptype, Color col) {
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
        } else if (isdigit(ch)) {
            decrement_square(static_cast<Square>(ch) - static_cast<Square>('0'));
        } else if (ch == '/') {
            continue;
        } else [[unlikely]] {
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
        game.castling_rights = 0;
    } else {
        for (const char ch : fen_castle) {
            switch (ch) {
                case 'K': {
                    game.castling_rights |= CASTLE_RIGHTS_WHITE_KINGSIDE;
                    break;
                }
                case 'Q': {
                    game.castling_rights |= CASTLE_RIGHTS_WHITE_QUEENSIDE;
                    break;
                }
                case 'k': {
                    game.castling_rights |= CASTLE_RIGHTS_BLACK_KINGSIDE;
                    break;
                }
                case 'q': {
                    game.castling_rights |= CASTLE_RIGHTS_BLACK_QUEENSIDE;
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

        if (MaybeSquare sq = square_from_algebraic(fen_ep.c_str()); sq.has_value()) {
            game.ep_square = sq;
        } else {
            WARN("Couldn't convert En Passant target square from algebraic notation.");
            return {};
        }
    }

    if (fen_pieces.size() >= 5) [[likely]] {
        game.halfmove_clock = atoi(fen_pieces[4].c_str());
    } else [[unlikely]] {
        game.halfmove_clock = 0;
    }

    if (fen_pieces.size() >= 6) [[likely]] {
        game.fullmoves = atoi(fen_pieces[5].c_str());
    } else [[unlikely]] {
        game.fullmoves = 1;
    }

    game.hash = create_game_hash(game);

    return game;
}

export std::optional<Game> game_from_fen(const std::string& fen)
{
    if (fen.length() == 0) [[unlikely]] {
        WARN("Passed empty FEN string to Game::create.");
        return {};
    } else if (fen.length() > 92) [[unlikely]] {
        WARN("Passed invalid FEN string that was too long to Game::create.");
        return {};
    } else [[likely]] {
        return game_from_fen_internal(fen);
    }
}

export std::string game_to_fen(const Game& game)
{
    std::string fen; fen.reserve(92);

    int empty_tally = 0;

    for (Square sq = 63; sq >= 0; sq--) {
        const bool wrapped_across_row = sq % 8 == 7;

        const std::optional<Piece> maybe_piece = get_piece_at(game.board, sq);

        if ((maybe_piece.has_value() || wrapped_across_row) && empty_tally > 0) {
            //assert(empty_tally <= 8);
            fen.push_back('0' + static_cast<char>(empty_tally));
            empty_tally = 0;
        }

        if (wrapped_across_row && sq < 63) {
            fen.push_back('/');
        }

        if (maybe_piece.has_value()) {
            const Piece& piece = *maybe_piece;

            char letter;
            switch (piece.type) {
                case PieceType::Pawn: {
                    letter = 'p';
                    break;
                }
                case PieceType::Knight: {
                    letter = 'n';
                    break;
                }
                case PieceType::Bishop: {
                    letter = 'b';
                    break;
                }
                case PieceType::Rook: {
                    letter = 'r';
                    break;
                }
                case PieceType::Queen: {
                    letter = 'q';
                    break;
                }
                case PieceType::King: {
                    letter = 'k';
                    break;
                }
            }

            if (piece.color == Color::White) {
                letter = toupper(letter);
            }

            fen.push_back(letter);
        } else {
            empty_tally++;
        }
    }

    if (empty_tally > 0) {
        fen.push_back('0' + static_cast<char>(empty_tally));
    }

    if (game.to_move == Color::White) {
        fen += " w ";
    } else {
        fen += " b ";
    }

    if (game.castling_rights == 0) {
        fen += "- ";
    } else {
        if (game.castling_rights & CASTLE_RIGHTS_WHITE_KINGSIDE) fen.push_back('K');
        if (game.castling_rights & CASTLE_RIGHTS_WHITE_QUEENSIDE) fen.push_back('Q');
        if (game.castling_rights & CASTLE_RIGHTS_BLACK_KINGSIDE) fen.push_back('k');
        if (game.castling_rights & CASTLE_RIGHTS_BLACK_QUEENSIDE) fen.push_back('q');
        fen.push_back(' ');
    }

    if (game.ep_square.has_value()) {
        fen += square_to_algebraic(*(game.ep_square));
    } else {
        fen.push_back('-');
    }

    fen.push_back(' ');
    fen += std::to_string(game.halfmove_clock);
    fen.push_back(' ');
    fen += std::to_string(game.fullmoves);

    return fen;
}

// tactical positions
// 3q1r1k/2r2ppp/2p1b3/1p2P2R/p1pP2Nb/P3Q2P/1P2B1P1/5RK1 w - - 5 27 (N -> F6)
