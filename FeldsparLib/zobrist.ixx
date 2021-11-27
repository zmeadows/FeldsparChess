export module zobrist;

#include "unstd/macros.h"

import prelude;
import bitboard;

import unstd.core;
import unstd.random;

export using GameHash = U64;

U64 PIECE_KEYS[12 * 64];
U64 CASTLE_KEYS[16];
U64 EN_PASSANT_KEYS[8];
U64 BLACK_TO_MOVE_KEY;

export void init_zobrist_hashing()
{
    for (S64 i = 0; i < 12; i++) {
        for (S64 j = 0; j < 64; j++) {
            PIECE_KEYS[i * 64 + j] = rand<U64>();
        }
    }

    for (S64 i = 0; i < 16; i++) {
        CASTLE_KEYS[i] = rand<U64>();
    }

    for (S64 i = 0; i < 8; i++) {
        EN_PASSANT_KEYS[i] = rand<U64>();
    }

    BLACK_TO_MOVE_KEY = rand<U64>();
}

export __ALWAYS_INLINE void hash_update_piece_square(GameHash& hash, Color color, PieceType ptype,
                                                   Square sq)
{
    hash ^= PIECE_KEYS[128 * static_cast<S64>(ptype) + static_cast<S64>(color) * 64 + sq];
}

export __ALWAYS_INLINE void hash_update_castling_rights(GameHash& hash, CastlingRights rights)
{
    hash ^= CASTLE_KEYS[rights];
}

export __ALWAYS_INLINE void hash_update_ep_square(GameHash& hash, Square epsq)
{
    hash ^= EN_PASSANT_KEYS[file_of(epsq)];
}

export __ALWAYS_INLINE void hash_update_color_to_move(GameHash& hash) { hash ^= BLACK_TO_MOVE_KEY; }
