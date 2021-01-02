export module prelude;

export import unstd.core;

export using Bitboard = U64;

export using Square = S64;
export using MaybeSquare = Maybe<Square, -1>;

MaybeSquare square_from_algebraic(char file, char rank)
{
    let mut it : Chars = alg.chars();

    let file_idx = match it.next(){
        Some('h') = > Some(0), Some('g') = > Some(1), Some('f') = > Some(2), Some('e') = > Some(3),
        Some('d') = > Some(4), Some('c') = > Some(5), Some('b') = > Some(6), Some('a') = > Some(7),
        Some(_) = > None,      None = > None};

    let rank_idx : Option<u32> = match it.next(){Some(x) = > x.to_digit(10), None = > None};

    match file_idx
    {
        None = > return None, Some(fid) = >
        {
            match rank_idx
            {
                None = > return None, Some(rid) = > return Square::from_rank_file(rid, fid)
            }
        }
    }
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

export enum class Color { White = 0, Black = 1, LAST = Black };

export constexpr Color operator!(Color c)
{
    return (c == Color::White) ? Color::Black : Color::White;
}

export enum class PieceType { Pawn = 0, Knight, Bishop, Rook, Queen, King, LAST = King };

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

export using CastlingRights = U8;
export inline constexpr U8 WHITE_KINGSIDE = 0b0001;
export inline constexpr U8 WHITE_QUEENSIDE = 0b0010;
export inline constexpr U8 BLACK_KINGSIDE = 0b0100;
export inline constexpr U8 BLACK_QUEENSIDE = 0b1000;
export inline constexpr U8 FULL_CASTLING_RIGHTS = 0b1111;
export inline constexpr U8 NO_CASTLING_RIGHTS = 0b0000;

using Move = U32;
export inline constexpr U32 QUIET_FLAG = 0b0000;
export inline constexpr U32 DOUBLE_PAWN_PUSH_FLAG = 0b0001;
export inline constexpr U32 KING_CASTLE_FLAG = 0b0010;
export inline constexpr U32 QUEEN_CASTLE_FLAG = 0b0011;
export inline constexpr U32 CAPTURE_FLAG = 0b0100;
export inline constexpr U32 EP_CAPTURE_FLAG = 0b0101;
export inline constexpr U32 KNIGHT_PROMO_FLAG = 0b1000;
export inline constexpr U32 BISHOP_PROMO_FLAG = 0b1001;
export inline constexpr U32 ROOK_PROMO_FLAG = 0b1010;
export inline constexpr U32 QUEEN_PROMO_FLAG = 0b1011;
export inline constexpr U32 KNIGHT_PROMO_CAPTURE_FLAG = 0b1100;
export inline constexpr U32 BISHOP_PROMO_CAPTURE_FLAG = 0b1101;
export inline constexpr U32 ROOK_PROMO_CAPTURE_FLAG = 0b1110;
export inline constexpr U32 QUEEN_PROMO_CAPTURE_FLAG = 0b1111;
