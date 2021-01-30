export module board;

import prelude;
import bitboard;

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

export __forceinline constexpr Bitboard get_unoccupied(const Board& board)
{
    return ~get_occupied(board);
}
