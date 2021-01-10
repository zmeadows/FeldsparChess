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

export __forceinline void board_reset_to_zero(Board& board)
{
    (void)memset(&board, 0, sizeof(Board));
}

export __forceinline constexpr Bitboard get_pieces(const Board& board, const PieceType p,
                                                   const Color c)
{
    const size_t idx = 2 * static_cast<size_t>(p) + static_cast<size_t>(c);
    return board[idx];
}

export __forceinline constexpr Bitboard& get_pieces_mut(Board& board, const PieceType p,
                                                        const Color c)
{
    const size_t idx = 2 * static_cast<size_t>(p) + static_cast<size_t>(c);
    return board[idx];
}

export __forceinline constexpr Bitboard get_occupied(const Board& board, const Color c)
{
    return board[12 + static_cast<size_t>(c)];
}

export __forceinline constexpr Bitboard& get_occupied_mut(Board& board, const Color c)
{
    return board[12 + static_cast<size_t>(c)];
}

export __forceinline constexpr Bitboard get_occupied(const Board& board)
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

export template <bool REMOVE_KING>
Bitboard attacked(const Board& board, Color attacking_color)
{
    using enum PieceType;

    const Color defending_color = !attacking_color;
    Bitboard attacked = BITBOARD_EMPTY;

    Bitboard defending_pieces = get_occupied(board, defending_color);

    if constexpr (REMOVE_KING) {
        defending_pieces &= bitboard_flipped(get_pieces(board, King, defending_color));
    }

    const Bitboard attacking_pieces = get_occupied(board, attacking_color);
    const Bitboard all_pieces = defending_pieces | attacking_pieces;

    bitboard_iter_squares(get_pieces(board, Pawn, attacking_color),
                          [&](Square sq) { attacked |= get_pawn_attacks(sq, attacking_color); });

    bitboard_iter_squares(get_pieces(board, Knight, attacking_color),
                          [&](Square sq) { attacked |= get_knight_attacks(sq); });

    bitboard_iter_squares(get_pieces(board, Bishop, attacking_color),
                          [&](Square sq) { attacked |= get_bishop_attacks(sq, all_pieces); });

    bitboard_iter_squares(get_pieces(board, Rook, attacking_color),
                          [&](Square sq) { attacked |= get_rook_attacks(sq, all_pieces); });

    bitboard_iter_squares(get_pieces(board, Queen, attacking_color),
                          [&](Square sq) { attacked |= get_queen_attacks(sq, all_pieces); });

    attacked |= get_king_attacks(bitboard_bsf(get_pieces(board, King, attacking_color)));

    return attacked;
}
