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

export float eval(const Game& game) { return 0.0; }
