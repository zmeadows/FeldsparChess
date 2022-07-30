export module attacks.pext;

import prelude;
import bitboard;
import attacks.classical;

import <algorithm>;
import <immintrin.h>;

#include "../unstd/macros.h"

static Bitboard* ATTACK_TABLE = new Bitboard[156800];
static Bitboard ROOK_MASKS[64];
static U64 ROOK_OFFSETS[64];
static Bitboard BISHOP_MASKS[64];
static U64 BISHOP_OFFSETS[64];

export void init_pext_tables(void)
{
    // precompute relevant occupancy masks
    for (Square sq = 0; sq < 64; sq++) {
        BISHOP_MASKS[sq] = INNER_SQUARES & get_bishop_attacks_classical(sq, BITBOARD_EMPTY);

        ROOK_MASKS[sq]  = ~CORNER_SQUARES & get_rook_attacks_classical(sq, BITBOARD_EMPTY);
        if (square_bitrep(sq) & INNER_SQUARES) {
            ROOK_MASKS[sq] &= INNER_SQUARES;
        }
    }

    U64 next_free_table_index = 0;

    for (Square sq = 0; sq < 64; sq++) {
        ROOK_OFFSETS[sq] = next_free_table_index;

        permutations(ROOK_MASKS[sq], [&](Bitboard occupied) {
            U64 mask_offset = _pext_u64(occupied, ROOK_MASKS[sq]);
            ATTACK_TABLE[ROOK_OFFSETS[sq] + mask_offset] = get_rook_attacks_classical(sq, occupied);
            next_free_table_index = std::max(ROOK_OFFSETS[sq] + mask_offset + 1, next_free_table_index);
        });
    }

    for (Square sq = 0; sq < 64; sq++) {
        BISHOP_OFFSETS[sq] = next_free_table_index;

        permutations(BISHOP_MASKS[sq], [&](Bitboard occupied) {
            U64 mask_offset = _pext_u64(occupied, BISHOP_MASKS[sq]);
            U64 table_idx = BISHOP_OFFSETS[sq] + mask_offset;
            ATTACK_TABLE[table_idx] = get_bishop_attacks_classical(sq, occupied);
            next_free_table_index = std::max(BISHOP_OFFSETS[sq] + mask_offset + 1, next_free_table_index);
        });
    }
}

export __ALWAYS_INLINE Bitboard get_bishop_attacks_pext(Square sq, Bitboard occupied)
{
    return ATTACK_TABLE[BISHOP_OFFSETS[sq] + _pext_u64(occupied, BISHOP_MASKS[sq])];
}

export __ALWAYS_INLINE Bitboard get_rook_attacks_pext(Square sq, Bitboard occupied)
{
    return ATTACK_TABLE[ROOK_OFFSETS[sq] + _pext_u64(occupied, ROOK_MASKS[sq])];
}


