export module board;

import prelude;
import bitboard;
import rays;
import tables;

import<cstring>;

export inline constexpr auto BOARD_SIZE = 14;
export using Board = Bitboard[BOARD_SIZE];
// Board[0] = White Pawns
// Board[1] = Black Pawns
// Board[2] = White Knights
// Board[3] = Black Knights
// Board[4] = White Bishops
// Board[5] = Black Bishops
// Board[6] = White Rook
// Board[7] = Black Rook
// Board[8] = White Queens
// Board[9] = Black Queens
// Board[10] = White King
// Board[11] = Black King
// Board[12] = White Occupied (OR of all White pieces)
// Board[13] = Black Occupied (OR of all Black pieces)

export inline constexpr Board NEW_BOARD = {
    0x000000000000FF00ULL, 71776119061217280ULL,  0x0000000000000042ULL, 0x4200000000000000ULL,
    0x0000000000000024ULL, 0x2400000000000000ULL, 0x0000000000000081ULL, 0x8100000000000000ULL,
    0x0000000000000010ULL, 0x1000000000000000ULL, 0x0000000000000008ULL, 0x0800000000000000ULL,
    0x000000000000FFFFULL, 0xFFFF000000000000ULL};

export inline void board_reset_to_zero(Board& board) { (void)memset(&board, 0, sizeof(Board)); }

export inline constexpr Bitboard get_pieces(const Board& board, const PieceType p, const Color c)
{
    const size_t idx = 2 * static_cast<size_t>(p) + static_cast<size_t>(c);
    return board[idx];
}

export inline constexpr Bitboard& get_pieces_mut(Board& board, const PieceType p, const Color c)
{
    const size_t idx = 2 * static_cast<size_t>(p) + static_cast<size_t>(c);
    return board[idx];
}

export inline constexpr Bitboard get_occupied(const Board& board, const Color c)
{
    return board[12 + static_cast<size_t>(c)];
}

export inline constexpr Bitboard& get_occupied_mut(Board& board, const Color c)
{
    return board[12 + static_cast<size_t>(c)];
}

export inline constexpr Bitboard get_occupied(const Board& board)
{
    return get_occupied(board, Color::White) || get_occupied(board, Color::Black);
}

export inline constexpr Bitboard attackers(const Board& board, Color color, Square sq)
{
    using enum PieceType;

    Bitboard attackers = BITBOARD_EMPTY;

    attackers |= get_pawn_attacks(sq, !color) & get_pieces(board, Pawn, color);
    attackers |= get_knight_attacks(sq) & get_pieces(board, Knight, color);
    attackers |= get_king_attacks(sq) & get_pieces(board, King, color);

    const Bitboard occupied = get_occupied(board);

    const Bitboard BQ = get_pieces(board, Queen, color) | get_pieces(board, Bishop, color);
    attackers |= get_bishop_attacks(sq, occupied) & BQ;

    const Bitboard RQ = get_pieces(board, Queen, color) | get_pieces(board, Rook, color);
    attackers |= get_rook_attacks(sq, occupied) & RQ;

    return attackers;
}
