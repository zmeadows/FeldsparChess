export module prelude;

export import unstd.core;

export using Bitboard = U64;

export using Square = S64;
export using MaybeSquare = Maybe<Square, -1>;

export using Move = U32;

export using CastlingRights = U8;
export inline constexpr U8 WHITE_KINGSIDE = 0b0001;
export inline constexpr U8 WHITE_QUEENSIDE = 0b0010;
export inline constexpr U8 WHITE_CASTLE_MASK = 0b0011;
export inline constexpr U8 BLACK_KINGSIDE = 0b0100;
export inline constexpr U8 BLACK_QUEENSIDE = 0b1000;
export inline constexpr U8 BLACK_CASTLE_MASK = 0b1100;
export inline constexpr U8 FULL_CASTLING_RIGHTS = 0b1111;
export inline constexpr U8 NO_CASTLING_RIGHTS = 0b0000;

// TODO: create bits_set({1,2,...}) function
export inline constexpr Bitboard WHITE_KINGSIDE_CASTLE_PATH = 1ULL << 1 | 1ULL << 2;
export inline constexpr Bitboard BLACK_KINGSIDE_CASTLE_PATH = 1ULL << 57 | 1ULL << 58;
export inline constexpr Bitboard BLACK_QUEENSIDE_CASTLE_PATH = 1ULL << 62 | 1ULL << 61 | 1ULL << 60;
export inline constexpr Bitboard WHITE_QUEENSIDE_CASTLE_PATH = 1ULL << 4 | 1ULL << 5 | 1ULL << 6;

export inline constexpr Bitboard BLACK_QUEENSIDE_CASTLE_SAFETY_REQ = 1ULL << 61 | 1ULL << 60;
export inline constexpr Bitboard WHITE_QUEENSIDE_CASTLE_SAFETY_REQ = 1ULL << 4 | 1ULL << 5;

export enum class Color { White = 0, Black = 1, LAST = Black };

export constexpr Color operator!(Color c)
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

export constexpr bool is_slider(const PieceType ptype)
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

export struct Piece {
    Color color;
    PieceType type;

    Piece() = delete;
    constexpr Piece(Color c, PieceType p) : color(c), type(p){};
};

export MaybeSquare from_algebraic(char file, char rank) { return {}; }
