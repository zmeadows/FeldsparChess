export module attacks.util;
#include "../unstd/macros.h"

import unstd.core;

import prelude;
import bitboard;
import board;
import quad;

import attacks.base;
import attacks;

// qsliders = {rq,rq,bq,bq}
QuadBitboard quadfill_sliders1(QuadBitboard qsliders, Bitboard empty)
{
    const QuadBitboard qmask = pack(NOT_H_FILE, BITBOARD_FULL, NOT_A_FILE, NOT_H_FILE);
    const QuadBitboard qshift = pack(1, 8, 7, 9);
    QuadBitboard qflood = qsliders;
    QuadBitboard qempty = pack(empty) & qmask;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= (qsliders << qshift) & qempty;
    return (qflood << qshift) & qmask;
}

// qsliders = {rq,rq,bq,bq}
QuadBitboard quadfill_sliders2(QuadBitboard qsliders, Bitboard empty)
{
    const QuadBitboard qmask = pack(NOT_A_FILE, BITBOARD_FULL, NOT_H_FILE, NOT_A_FILE);
    const QuadBitboard qshift = pack(1, 8, 7, 9);
    QuadBitboard qflood = qsliders;
    QuadBitboard qempty = pack(empty) & qmask;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= (qsliders >> qshift) & qempty;
    return (qflood >> qshift) & qmask;
}

export struct AttackedFlags {
    Color ATTACKING_COLOR = Color::White;
    bool REMOVE_KING = false;
    bool USE_QUAD_FILL = false;
};

export template <AttackedFlags FLAGS = AttackedFlags()>
__FLATTEN_CALLS Bitboard attacked(const Board& board)
{
    using enum PieceType;

    constexpr Color ATTACKING_COLOR = FLAGS.ATTACKING_COLOR;
    constexpr Color DEFENDING_COLOR = !ATTACKING_COLOR;
    Bitboard attacked = BITBOARD_EMPTY;

    Bitboard defending_pieces = get_occupied(board, DEFENDING_COLOR);

    if constexpr (FLAGS.REMOVE_KING) {
        defending_pieces ^= get_pieces(board, King, DEFENDING_COLOR);
    }

    const Bitboard attacking_pieces = get_occupied(board, ATTACKING_COLOR);
    const Bitboard all_pieces = defending_pieces | attacking_pieces;

    const Bitboard attacking_pawns = get_pieces(board, Pawn, ATTACKING_COLOR);

    if constexpr (ATTACKING_COLOR == Color::White) {
        attacked |= bitboard_shifted(attacking_pawns, Direction::NorthWest);
        attacked |= bitboard_shifted(attacking_pawns, Direction::NorthEast);
    } else {
        attacked |= bitboard_shifted(attacking_pawns, Direction::SouthWest);
        attacked |= bitboard_shifted(attacking_pawns, Direction::SouthEast);
    }

    serialize(get_pieces(board, Knight, ATTACKING_COLOR),
              [&](Square sq) { attacked |= get_knight_moves(sq); });

    serialize(get_pieces(board, King, ATTACKING_COLOR),
              [&](Square sq) { attacked |= get_king_moves(sq); });

    if constexpr (FLAGS.USE_QUAD_FILL) {
        const Bitboard empty = bitboard_flipped(all_pieces);

        const Bitboard q = get_pieces(board, Queen, ATTACKING_COLOR);
        const Bitboard rq = q | get_pieces(board, Rook, ATTACKING_COLOR);
        const Bitboard bq = q | get_pieces(board, Bishop, ATTACKING_COLOR);
        const QuadBitboard sliders = pack(rq, rq, bq, bq);

        QuadBitboard sliding_attacks = quadfill_sliders1(sliders, empty);
        sliding_attacks |= quadfill_sliders2(sliders, empty);
        attacked |= reduceOR(sliding_attacks);
    } else {
        serialize(get_pieces(board, Bishop, ATTACKING_COLOR),
                  [&](Square sq) { attacked |= get_bishop_attacks(sq, all_pieces); });

        serialize(get_pieces(board, Rook, ATTACKING_COLOR),
                  [&](Square sq) { attacked |= get_rook_attacks(sq, all_pieces); });

        serialize(get_pieces(board, Queen, ATTACKING_COLOR),
                  [&](Square sq) { attacked |= get_queen_attacks(sq, all_pieces); });
    }

    return attacked;
}

export __FLATTEN_CALLS Bitboard attackers(const Board& board, Color color,
                                                                Square sq)
{
    using enum PieceType;

    Bitboard attackers = BITBOARD_EMPTY;

    attackers |= get_pawn_attacks(sq, !color) & get_pieces(board, Pawn, color);
    attackers |= get_knight_moves(sq) & get_pieces(board, Knight, color);
    attackers |= get_king_moves(sq) & get_pieces(board, King, color);

    const Bitboard occupied = get_occupied(board);

    const Bitboard Q = get_pieces(board, Queen, color);
    const Bitboard BQ = Q | get_pieces(board, Bishop, color);
    attackers |= get_bishop_attacks(sq, occupied) & BQ;

    const Bitboard RQ = Q | get_pieces(board, Rook, color);
    attackers |= get_rook_attacks(sq, occupied) & RQ;

    return attackers;
}
