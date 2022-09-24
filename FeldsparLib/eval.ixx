export module eval;

#include "unstd/macros.h"
import prelude;

import bitboard;
import board;
import game;

import<utility>;

/*
export struct EvalParams {
    F64 pawn_value = 1.0;
    F64 knight_value = 2.9;
    F64 bishop_value = 3.1;
    F64 rook_value = 5.0;
    F64 queen_value = 9.0;

    F64 pawn_phase = 0.25f;
    F64 knight_phase = 1.2f;
    F64 bishop_phase = 0.8f;
    F64 rook_phase = 2.5f;
    F64 queen_phase = 4.f;

    struct {
        F64 to_move_bonus = 0.1;

        F64 pawn_sq_defend = 0.04;
        F64 bishop_sq = 0.02;
        F64 knight_sq = 0.03;
        F64 rook_sq = 0.025;
        F64 queen_sq = 0.015;

        F64 center_control = 0.01;
    } midgame;
};

struct MobilityMask {
    Bitboard pawns = BITBOARD_EMPTY;
    Bitboard minor = BITBOARD_EMPTY;
    Bitboard full = BITBOARD_EMPTY;
};

struct PieceSquareWeights {
    F64 knight[64] = {1.0};
    F64 bishop[64] = {1.0};
    F64 rook[64] = {1.0};
    F64 queen[64] = {1.0};
};

struct EvalContext {
    MobilityMask mmask;
    PieceSquareWeights psw;
};

// TODO: use auto return type to return Bitboard or AttackMask struct
template <Color attacking_color>
AttackMask compute_attack_mask(const Board& board)
{
}

float eval_midgame(const Game& game, const EvalParams& p) { return 0; }
float eval_endgame(const Game& game, const EvalParams& p) { return 0; }

float eval(const Game& game, const EvalParams& p)
{
    using enum PieceType;

    S64 pvalue = 0;
    float phase = 16 * p.pawn_phase + 4 * p.knight_phase + 4 * p.bishop_phase + 4 * p.rook_phase +
                  2 * p.queen_phase;

    { // compute phase and static piece value scores
        const S64 white_pawns = bitboard_popcount(get_pieces(board, Pawn, Color::White));
        const S64 white_knights = bitboard_popcount(get_pieces(board, Knight, Color::White));
        const S64 white_bishops = bitboard_popcount(get_pieces(board, Bishop, Color::White));
        const S64 white_rooks = bitboard_popcount(get_pieces(board, Rook, Color::White));
        const S64 white_queens = bitboard_popcount(get_pieces(board, Queen, Color::White));
        const S64 black_pawns = bitboard_popcount(get_pieces(board, Pawn, Color::Black));
        const S64 black_knights = bitboard_popcount(get_pieces(board, Knight, Color::Black));
        const S64 black_bishops = bitboard_popcount(get_pieces(board, Bishop, Color::Black));
        const S64 black_rooks = bitboard_popcount(get_pieces(board, Rook, Color::Black));
        const S64 black_queens = bitboard_popcount(get_pieces(board, Queen, Color::Black));

        pvalue += p.pawn_value * (white_pawns - black_pawns);
        pvalue += p.knight_value * (white_knights - black_knights);
        pvalue += p.bishop_value * (white_bishops - black_bishops);
        pvalue += p.rook_value * (white_rooks - black_rooks);
        pvalue += p.queen_value * (white_queens - black_queens);

        phase -= (white_pawns + black_pawns) * p.pawn_phase;
        phase -= (white_knights + black_knights) * p.knight_phase;
        phase -= (white_bishops + black_bishops) * p.bishop_phase;
        phase -= (white_rooks + black_rooks) * p.rook_phase;
        phase -= (white_queens + black_queens) * p.queen_phase;
    }

    const Color attacking_color = game.to_move;
    const Color defending_color = !attacking_color;
    const Bitboard attacking_pieces = get_occupied(board, attacking_color);
    const Bitboard defending_pieces = get_occupied(board, defending_color);
    const Bitboard all_pieces = defending_pieces | attacking_pieces;

    using enum PieceType;
    EvalContext attacking_ctx, defending_ctx;

    constexpr auto initialize_midgame_ctx = [](EvalContext& ctx, Color col, const EvalParams& p) {
        serialize(get_pieces(board, Pawn, col), [&](Square pawn_sq) {
            const Bitboard pawn_attack_mask = get_pawn_attacks(pawn_sq, col);
            ctx.mask.pawns |= pawn_attack_mask;
            serialize(pawn_attack_mask, [&](Square attack_sq) { ctx.psq.pawns[attack_sq] });
        });

        mask.full |= mask.pawns;

        serialize(get_pieces(board, Knight, attacking_color),
                  [&](Square sq) { mask.minor |= get_knight_moves(sq); });

        serialize(get_pieces(board, Bishop, attacking_color),
                  [&](Square sq) { mask.minor |= get_bishop_attacks(sq, all_pieces); });

        serialize(get_pieces(board, Rook, attacking_color),
                  [&](Square sq) { mask.minor |= get_rook_attacks(sq, all_pieces); });

        mask.full |= mask.minor;

        serialize(get_pieces(board, Queen, attacking_color),
                  [&](Square sq) { mask.full |= get_queen_attacks(sq, all_pieces); });

        serialize(get_pieces(board, King, attacking_color),
                  [&](Square sq) { mask.full |= get_king_moves(sq); });
    };

    return pvalue + (1.f - phase) * mg + phase * eg;
}
*/
