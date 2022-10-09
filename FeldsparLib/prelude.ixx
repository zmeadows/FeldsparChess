export module prelude;

#include "unstd/macros.h"
export import unstd.core;
export import <cassert>;

import <cstring>;

export constexpr const char* FELDSPAR_VERSION = "0.0.1";

export using Bitboard = U64;

export using Square = S64;
export using MaybeSquare = Maybe<Square, -1>;

export constexpr __ALWAYS_INLINE bool is_valid_square(Square sq) { return sq >= 0 && sq < 64; }
export __ALWAYS_INLINE constexpr S64 rank_of(Square sq) { return 1 + (sq / 8); }
export __ALWAYS_INLINE constexpr S64 file_of(Square sq) { return 8 - (sq % 8); }
export __ALWAYS_INLINE constexpr Square from_rank_file(S64 rank, S64 file)
{
    const Square sq = rank * 8 - file;
    // assert(sq >= 0 && sq <= 63);
    return sq;
}

export using Move = U32;

export using CastlingRights = U8;
export inline constexpr CastlingRights CASTLE_RIGHTS_WHITE_KINGSIDE = 0b0001;
export inline constexpr CastlingRights CASTLE_RIGHTS_WHITE_QUEENSIDE = 0b0010;
export inline constexpr CastlingRights CASTLE_RIGHTS_WHITE_BOTH = 0b0011;
export inline constexpr CastlingRights CASTLE_RIGHTS_BLACK_KINGSIDE = 0b0100;
export inline constexpr CastlingRights CASTLE_RIGHTS_BLACK_QUEENSIDE = 0b1000;
export inline constexpr CastlingRights CASTLE_RIGHTS_BLACK_BOTH = 0b1100;

export __ALWAYS_INLINE void remove_castling_rights(CastlingRights& rights, CastlingRights to_remove)
{
    rights &= ~(to_remove);
}

// TODO: use bits_set({1,2,...}) function
export inline constexpr Bitboard WHITE_KINGSIDE_CASTLE_PATH = 1ULL << 1 | 1ULL << 2;
export inline constexpr Bitboard BLACK_KINGSIDE_CASTLE_PATH = 1ULL << 57 | 1ULL << 58;
export inline constexpr Bitboard BLACK_QUEENSIDE_CASTLE_PATH = 1ULL << 62 | 1ULL << 61 | 1ULL << 60;
export inline constexpr Bitboard WHITE_QUEENSIDE_CASTLE_PATH = 1ULL << 4 | 1ULL << 5 | 1ULL << 6;
export inline constexpr Bitboard BLACK_QUEENSIDE_CASTLE_SAFETY_REQ = 1ULL << 61 | 1ULL << 60;
export inline constexpr Bitboard WHITE_QUEENSIDE_CASTLE_SAFETY_REQ = 1ULL << 4 | 1ULL << 5;

export enum class Color { White = 0, Black = 1, LAST = Black };

export constexpr __ALWAYS_INLINE Color operator!(Color c)
{
    return (c == Color::White) ? Color::Black : Color::White;
}

export enum class Direction {
    North,
    South,
    East,
    West,
    NorthEast,
    NorthWest,
    SouthEast,
    SouthWest,
    LAST = SouthWest
};

export enum class PieceType { Pawn = 0, Knight, Bishop, Rook, Queen, King, LAST = King };

using MaybePieceType = Maybe<PieceType, PieceType::LAST>;

export struct Piece {
    Color color;
    PieceType type;
};

export constexpr __ALWAYS_INLINE bool is_slider(const PieceType ptype)
{
    switch (ptype) {
        case PieceType::Bishop:
            return true;
        case PieceType::Rook:
            return true;
        case PieceType::Queen:
            return true;
        default:
            return false;
    }
}

export inline constexpr Square h1 = 0 * 8 + 0;
export inline constexpr Square g1 = 0 * 8 + 1;
export inline constexpr Square f1 = 0 * 8 + 2;
export inline constexpr Square e1 = 0 * 8 + 3;
export inline constexpr Square d1 = 0 * 8 + 4;
export inline constexpr Square c1 = 0 * 8 + 5;
export inline constexpr Square b1 = 0 * 8 + 6;
export inline constexpr Square a1 = 0 * 8 + 7;
export inline constexpr Square h2 = 1 * 8 + 0;
export inline constexpr Square g2 = 1 * 8 + 1;
export inline constexpr Square f2 = 1 * 8 + 2;
export inline constexpr Square e2 = 1 * 8 + 3;
export inline constexpr Square d2 = 1 * 8 + 4;
export inline constexpr Square c2 = 1 * 8 + 5;
export inline constexpr Square b2 = 1 * 8 + 6;
export inline constexpr Square a2 = 1 * 8 + 7;
export inline constexpr Square h3 = 2 * 8 + 0;
export inline constexpr Square g3 = 2 * 8 + 1;
export inline constexpr Square f3 = 2 * 8 + 2;
export inline constexpr Square e3 = 2 * 8 + 3;
export inline constexpr Square d3 = 2 * 8 + 4;
export inline constexpr Square c3 = 2 * 8 + 5;
export inline constexpr Square b3 = 2 * 8 + 6;
export inline constexpr Square a3 = 2 * 8 + 7;
export inline constexpr Square h4 = 3 * 8 + 0;
export inline constexpr Square g4 = 3 * 8 + 1;
export inline constexpr Square f4 = 3 * 8 + 2;
export inline constexpr Square e4 = 3 * 8 + 3;
export inline constexpr Square d4 = 3 * 8 + 4;
export inline constexpr Square c4 = 3 * 8 + 5;
export inline constexpr Square b4 = 3 * 8 + 6;
export inline constexpr Square a4 = 3 * 8 + 7;
export inline constexpr Square h5 = 4 * 8 + 0;
export inline constexpr Square g5 = 4 * 8 + 1;
export inline constexpr Square f5 = 4 * 8 + 2;
export inline constexpr Square e5 = 4 * 8 + 3;
export inline constexpr Square d5 = 4 * 8 + 4;
export inline constexpr Square c5 = 4 * 8 + 5;
export inline constexpr Square b5 = 4 * 8 + 6;
export inline constexpr Square a5 = 4 * 8 + 7;
export inline constexpr Square h6 = 5 * 8 + 0;
export inline constexpr Square g6 = 5 * 8 + 1;
export inline constexpr Square f6 = 5 * 8 + 2;
export inline constexpr Square e6 = 5 * 8 + 3;
export inline constexpr Square d6 = 5 * 8 + 4;
export inline constexpr Square c6 = 5 * 8 + 5;
export inline constexpr Square b6 = 5 * 8 + 6;
export inline constexpr Square a6 = 5 * 8 + 7;
export inline constexpr Square h7 = 6 * 8 + 0;
export inline constexpr Square g7 = 6 * 8 + 1;
export inline constexpr Square f7 = 6 * 8 + 2;
export inline constexpr Square e7 = 6 * 8 + 3;
export inline constexpr Square d7 = 6 * 8 + 4;
export inline constexpr Square c7 = 6 * 8 + 5;
export inline constexpr Square b7 = 6 * 8 + 6;
export inline constexpr Square a7 = 6 * 8 + 7;
export inline constexpr Square h8 = 7 * 8 + 0;
export inline constexpr Square g8 = 7 * 8 + 1;
export inline constexpr Square f8 = 7 * 8 + 2;
export inline constexpr Square e8 = 7 * 8 + 3;
export inline constexpr Square d8 = 7 * 8 + 4;
export inline constexpr Square c8 = 7 * 8 + 5;
export inline constexpr Square b8 = 7 * 8 + 6;
export inline constexpr Square a8 = 7 * 8 + 7;

// TODO: replace with simple version calling 'a' + file, if it can be constexpr
export constexpr const char* square_to_algebraic(Square sq)
{
    switch (sq) {
        case h1:
            return "h1";
        case g1:
            return "g1";
        case f1:
            return "f1";
        case e1:
            return "e1";
        case d1:
            return "d1";
        case c1:
            return "c1";
        case b1:
            return "b1";
        case a1:
            return "a1";
        case h2:
            return "h2";
        case g2:
            return "g2";
        case f2:
            return "f2";
        case e2:
            return "e2";
        case d2:
            return "d2";
        case c2:
            return "c2";
        case b2:
            return "b2";
        case a2:
            return "a2";
        case h3:
            return "h3";
        case g3:
            return "g3";
        case f3:
            return "f3";
        case e3:
            return "e3";
        case d3:
            return "d3";
        case c3:
            return "c3";
        case b3:
            return "b3";
        case a3:
            return "a3";
        case h4:
            return "h4";
        case g4:
            return "g4";
        case f4:
            return "f4";
        case e4:
            return "e4";
        case d4:
            return "d4";
        case c4:
            return "c4";
        case b4:
            return "b4";
        case a4:
            return "a4";
        case h5:
            return "h5";
        case g5:
            return "g5";
        case f5:
            return "f5";
        case e5:
            return "e5";
        case d5:
            return "d5";
        case c5:
            return "c5";
        case b5:
            return "b5";
        case a5:
            return "a5";
        case h6:
            return "h6";
        case g6:
            return "g6";
        case f6:
            return "f6";
        case e6:
            return "e6";
        case d6:
            return "d6";
        case c6:
            return "c6";
        case b6:
            return "b6";
        case a6:
            return "a6";
        case h7:
            return "h7";
        case g7:
            return "g7";
        case f7:
            return "f7";
        case e7:
            return "e7";
        case d7:
            return "d7";
        case c7:
            return "c7";
        case b7:
            return "b7";
        case a7:
            return "a7";
        case h8:
            return "h8";
        case g8:
            return "g8";
        case f8:
            return "f8";
        case e8:
            return "e8";
        case d8:
            return "d8";
        case c8:
            return "c8";
        case b8:
            return "b8";
        case a8:
            return "a8";
        default:
            return "UNKNOWN";
    }
}

export __ALWAYS_INLINE MaybeSquare square_from_algebraic(const char* alg)
{
    if (strlen(alg) != 2) [[unlikely]] {
        return {};
    }

    const S64 rank = static_cast<S64>(alg[1] - '0');
    const S64 file = static_cast<S64>(alg[0] - 'a' + 1);

    return from_rank_file(rank, file);
}
