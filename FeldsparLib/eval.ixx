export module eval;

import prelude;
import bitboard;
import board;
import game;

struct EvalParams {
    S64 pawn_value = 100;
    S64 knight_value = 320;
    S64 bishop_value = 330;
    S64 rook_value = 500;
    S64 queen_value = 900;
};

S64 eval_piece_value(const Board& board, const EvalParams& p)
{
    using enum PieceType;

    S64 score = 0;

    score += p.pawn_value * bitboard_popcount(get_pieces(board, Pawn, Color::White));
    score += p.knight_value * bitboard_popcount(get_pieces(board, Knight, Color::White));
    score += p.bishop_value * bitboard_popcount(get_pieces(board, Bishop, Color::White));
    score += p.rook_value * bitboard_popcount(get_pieces(board, Rook, Color::White));
    score += p.queen_value * bitboard_popcount(get_pieces(board, Queen, Color::White));

    score -= p.pawn_value * bitboard_popcount(get_pieces(board, Pawn, Color::Black));
    score -= p.knight_value * bitboard_popcount(get_pieces(board, Knight, Color::Black));
    score -= p.bishop_value * bitboard_popcount(get_pieces(board, Bishop, Color::Black));
    score -= p.rook_value * bitboard_popcount(get_pieces(board, Rook, Color::Black));
    score -= p.queen_value * bitboard_popcount(get_pieces(board, Queen, Color::Black));

    return score;
}

export S64 eval(const Game& game, const EvalParams& p)
{
    S64 score = 0;

    score += eval_piece_value(game.board, p);

    return score;
}
