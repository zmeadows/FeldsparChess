export module zobrist;

import prelude;
import bitboard;
import game;

import unstd.array;
import unstd.core;
import unstd.random;

export using GameHash = U64;

Array<U64, 12 * 64> PIECE_KEYS;
Array<U64, 16> CASTLE_KEYS;
Array<U64, 8> EN_PASSANT_KEYS;
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

// export GameHash hash_game(const Game& game) { return 0; }
//
// export __forceinline void hash_update_piece_square(GameHash& hash, PieceType ptype, Square sq)
// {
//     hash ^= PIECE_KEYS[2 * static_cast<S64>(ptype) + sq];
// }
//
// export __forceinline void hash_update_black_to_move(GameHash& hash) { hash ^= BLACK_TO_MOVE_KEY;
// }
//
// export __forceinline void hash_update_castling_rights(GameHash& hash, CastlingRights rights)
// {
//     hash ^= CASTLE_KEYS[rights];
// }
//
// export __forceinline void hash_update_ep_square(GameHash& hash, Square epsq)
// {
//     hash ^= EN_PASSANT_KEYS[file_of(epsq)];
// }
