export module eval;

import prelude;
import bitboard;
import board;
import game;

#include "unstd/macros.h"

import<utility>;

export struct EvalParams {
    S64 pawn_value = 100;
    S64 knight_value = 320;
    S64 bishop_value = 330;
    S64 rook_value = 500;
    S64 queen_value = 900;

    float pawn_phase = 0.15f;
    float knight_phase = 1.1f;
    float bishop_phase = 0.9f;
    float rook_phase = 2.5f;
    float queen_phase = 4.f;
};

struct MobilityMasks {
    Bitboard minor_pieces = BITBOARD_FULL;
    Bitboard queen = BITBOARD_FULL;
};

struct MobilityWeights {
    S64 knight[64];
    S64 bishop[64];
    S64 rook[64];
    S64 queen[64];
};

template <Color C>
MobilityMasks compute_mobility_masks(const Board& board, const EvalParams& p)
{
    MobilityMasks mask;
    return mask;
}

template <Color C>
S64 compute_mobility_bonus(const Board& board, const EvalParams& p)
{
    static thread_local MobilityWeights weights;
    weights.knight = {0};
    weights.bishop = {0};
    weights.rook = {0};
    weights.queen = {0};

    return 0;
}

std::pair<float, float> compute_piece_value_and_phase(const Board& board, const EvalParams& p)
{
    using enum PieceType;

    const float max_phase = 16 * p.pawn_phase + 4 * p.knight_phase + 4 * p.bishop_phase +
                            4 * p.rook_phase + 2 * p.queen_phase;

    S64 pvalue = 0;
    float phase = max_phase;

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

    return {static_cast<float>(pvalue), phase / max_phase};
}

float eval_midgame(const Game& game, const EvalParams& p) { return 0; }
float eval_endgame(const Game& game, const EvalParams& p) { return 0; }

// TODO: if phase above a certain amount, skip midgame eval altogether
template <Color friendly_color, bool DEBUG_PRINT = false>
export float eval(const Game& game, const EvalParams& p)
{
    const auto [pvalue, phase] = compute_piece_value_and_phase(game.board, p);

    const float mg = eval_midgame(game, p);
    const float eg = eval_endgame(game, p);

    return pvalue + (1.f - phase) * mg + phase * eg;
}
