export module attacks.magic;

#include "../unstd/macros.h"

import prelude;
import bitboard;

export import attacks.base;

struct MagicSquare {
    Bitboard* table_chunk = nullptr;
    Bitboard mask = 0;
    U64 magic = 0;
    S64 shift = 0;
};

export template <typename Callable>
__ALWAYS_INLINE void for_each_mask_permutation(Bitboard mask, Callable&& f)
{
    // TODO: this is just a stub, implement the real permutations
    while (mask != 0) {
        f(mask);
        mask &= (mask - 1);
    }
}

// constexpr Array<MagicSquare, 64> generate_bishop_magics()
// {
//     Array<MagicSquare, 64> magics;
//
//     for (Square sq = 0; sq < 64; sq++) {
//         MagicSquare magic;
//         magic.mask = INNER_SQUARES & bishop_mask_ex(sq);
//     }
//
//     return magics;
// }

export void fill_magic_attack_table(void) {}

Bitboard rook_attacks(Bitboard occupied, Square sq) { return 0; }

Bitboard bishop_attacks(Bitboard occupied, Square sq) { return 0; }

Bitboard queen_attacks(Bitboard occupied, Square sq)
{
    return rook_attacks(occupied, sq) | bishop_attacks(occupied, sq);
}
