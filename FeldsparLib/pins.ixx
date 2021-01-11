export module pins;

import prelude;
import bitboard;
import rays;
import board;

import<cassert>;

// TODO: perhaps replace these with small stack-based hash tables?
export struct Pins {
    Bitboard diagonal = BITBOARD_EMPTY;
    Bitboard nondiagonal = BITBOARD_EMPTY;
    Bitboard diagonal_constraints[64];
    Bitboard nondiagonal_constraints[64];

    static __forceinline Pins create(const Board& board, const Color friendly_color);
};

export __forceinline Pins Pins::create(const Board& board, const Color friendly_color)
{
    using enum PieceType;

    struct Pins pins;

    const Color opponent_color = !friendly_color;
    const Bitboard friendly_pieces = get_occupied(board, friendly_color);
    const Bitboard opponent_pieces = get_occupied(board, !friendly_color);
    const Bitboard occupied_squares = friendly_pieces | opponent_pieces;
    const Square king_square = get_pieces(board, King, friendly_color);

    { // Find non-diagonal pins
        const Bitboard op_rq =
            get_pieces(board, Rook, opponent_color) | get_pieces(board, Queen, opponent_color);

        const Bitboard pinners =
            xray_rook_attacks(occupied_squares, friendly_pieces, king_square) & op_rq;

        serialize(pinners, [&](Square pinner_square) {
            const Bitboard connecting_bits = ray_between_squares(king_square, pinner_square);
            const Bitboard pinned_bit = connecting_bits & friendly_pieces;
            assert(bitboard_popcount(pinned_bit) == 1);
            pins.nondiagonal_constraints[bitboard_bsf(pinned_bit)] = connecting_bits;
            pins.nondiagonal |= pinned_bit;
        });
    }

    { // Find diagonal pins
        const Bitboard op_bq =
            get_pieces(board, Bishop, opponent_color) | get_pieces(board, Queen, opponent_color);

        const Bitboard pinners =
            xray_bishop_attacks(occupied_squares, friendly_pieces, king_square) & op_bq;

        serialize(pinners, [&](Square pinner_square) {
            const Bitboard connecting_bits = ray_between_squares(king_square, pinner_square);
            const Bitboard pinned_bit = connecting_bits & friendly_pieces;
            assert(bitboard_popcount(pinned_bit) == 1);
            pins.diagonal_constraints[bitboard_bsf(pinned_bit)] = connecting_bits;
            pins.diagonal |= pinned_bit;
        });
    }

    return pins;
}
