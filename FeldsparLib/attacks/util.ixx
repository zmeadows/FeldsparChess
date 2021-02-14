export module attacks.util;

import prelude;
import bitboard;
import board;
import quad;

#include "../unstd/macros.h"

import<cstdio>;

import attacks.classical;

// qsliders = {rq,rq,bq,bq}
// TODO: replace -1 with BITBOARD_FULL
template <bool DEBUG_PRINT = false>
QuadBitboard quadfill_sliders1(QuadBitboard qsliders, Bitboard empty)
{
    const QuadBitboard qmask = pack(NOT_H_FILE, BITBOARD_FULL, NOT_A_FILE, NOT_H_FILE);
    const QuadBitboard qshift = pack(1, 8, 7, 9);
    QuadBitboard qflood = qsliders;
    QuadBitboard qempty = pack(empty) & qmask;
    if (DEBUG_PRINT) DEBUG_PRINT_QBB2(qflood, "qflood start");
    qflood |= qsliders = (qsliders << qshift) & qempty;
    if (DEBUG_PRINT) DEBUG_PRINT_QBB2(qflood, "qflood 1");
    qflood |= qsliders = (qsliders << qshift) & qempty;
    if (DEBUG_PRINT) DEBUG_PRINT_QBB2(qflood, "qflood 2");
    qflood |= qsliders = (qsliders << qshift) & qempty;
    if (DEBUG_PRINT) DEBUG_PRINT_QBB2(qflood, "qflood 3");
    qflood |= qsliders = (qsliders << qshift) & qempty;
    if (DEBUG_PRINT) DEBUG_PRINT_QBB2(qflood, "qflood 4");
    qflood |= qsliders = (qsliders << qshift) & qempty;
    if (DEBUG_PRINT) DEBUG_PRINT_QBB2(qflood, "qflood 5");
    qflood |= (qsliders << qshift) & qempty;
    if (DEBUG_PRINT) DEBUG_PRINT_QBB2(qflood, "qflood 6");
    return (qflood << qshift) & qmask;
}

// qsliders = {rq,rq,bq,bq}
template <bool DEBUG_PRINT = false>
QuadBitboard quadfill_sliders2(QuadBitboard qsliders, Bitboard empty)
{
    if (DEBUG_PRINT) {
        printf("west_sout_soEa_soWe_Attacks:\n");
    }
    const QuadBitboard qmask = pack(NOT_A_FILE, -1, NOT_H_FILE, NOT_A_FILE);
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

export template <bool REMOVE_KING, bool USE_QUAD_FILL = false, bool DEBUG_PRINT = false>
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

    const Bitboard attacking_pawns = get_pieces(board, Pawn, attacking_color);

    if (attacking_color == Color::White) {
        attacked |= bitboard_shifted(attacking_pawns, Direction::NorthWest);
        attacked |= bitboard_shifted(attacking_pawns, Direction::NorthEast);
    } else {
        attacked |= bitboard_shifted(attacking_pawns, Direction::SouthWest);
        attacked |= bitboard_shifted(attacking_pawns, Direction::SouthEast);
    }

    serialize(get_pieces(board, Knight, attacking_color),
              [&](Square sq) { attacked |= get_knight_moves(sq); });

    serialize(get_pieces(board, King, attacking_color),
              [&](Square sq) { attacked |= get_king_moves(sq); });

    if constexpr (USE_QUAD_FILL) {
        const Bitboard empty = bitboard_flipped(all_pieces);

        const Bitboard q = get_pieces(board, Queen, attacking_color);
        const Bitboard rq = q | get_pieces(board, Rook, attacking_color);
        const Bitboard bq = q | get_pieces(board, Bishop, attacking_color);
        const QuadBitboard sliders = pack(rq, rq, bq, bq);

        QuadBitboard sliding_attacks = quadfill_sliders1<DEBUG_PRINT>(sliders, empty);
        sliding_attacks |= quadfill_sliders2<DEBUG_PRINT>(sliders, empty);
        attacked |= reduceOR(sliding_attacks);
    } else {
        serialize(get_pieces(board, Bishop, attacking_color),
                  [&](Square sq) { attacked |= get_bishop_attacks(sq, all_pieces); });

        serialize(get_pieces(board, Rook, attacking_color),
                  [&](Square sq) { attacked |= get_rook_attacks(sq, all_pieces); });

        serialize(get_pieces(board, Queen, attacking_color),
                  [&](Square sq) { attacked |= get_queen_attacks(sq, all_pieces); });
    }

    return attacked;
}

export constexpr Bitboard attackers(const Board& board, Color color, Square sq)
{
    using enum PieceType;

    Bitboard attackers = BITBOARD_EMPTY;

    attackers |= get_pawn_attacks(sq, !color) & get_pieces(board, Pawn, color);
    attackers |= get_knight_moves(sq) & get_pieces(board, Knight, color);
    attackers |= get_king_moves(sq) & get_pieces(board, King, color);

    const Bitboard occupied = get_occupied(board);

    const Bitboard BQ = get_pieces(board, Queen, color) | get_pieces(board, Bishop, color);
    attackers |= get_bishop_attacks(sq, occupied) & BQ;

    const Bitboard RQ = get_pieces(board, Queen, color) | get_pieces(board, Rook, color);
    attackers |= get_rook_attacks(sq, occupied) & RQ;

    return attackers;
}
