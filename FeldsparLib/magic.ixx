export module attacks.magic;

import prelude;
import bitboard;
import attacks.base;

import unstd.core;
import unstd.array;

constexpr const Bitboard INNER_SQUARES = BB("00000000"
                                            "01111110"
                                            "01111110"
                                            "01111110"
                                            "01111110"
                                            "01111110"
                                            "01111110"
                                            "00000000");

constexpr Array<Bitboard, 64> generate_attack_table()
{
    Array<Bitboard, 64> table;
    return table;
}

struct MagicSquare {
    Bitboard* table_chunk = nullptr;
    Bitboard mask = 0;
    U64 magic = 0;
    S64 shift = 0;
};

// TODO: this is just a stub, implement the real permutations
export template <typename Callable>
inline void for_each_mask_permutation(Bitboard mask, Callable&& f)
{
    while (mask != 0) {
        f(mask);
        mask &= (mask - 1);
    }
}

constexpr Array<MagicSquare, 64> generate_bishop_magics()
{
    Array<MagicSquare, 64> magics;

    for (Square sq = 0; sq < 64; sq++) {
        MagicSquare magic;
        magic.mask = INNER_SQUARES & bishop_mask_ex(sq);
    }

    return magics;
}

export void fill_magic_attack_table(void) {}

Bitboard rook_attacks(Bitboard occupied, Square sq) { return 0; }

Bitboard bishop_attacks(Bitboard occupied, Square sq) { return 0; }

Bitboard queen_attacks(Bitboard occupied, Square sq)
{
    return rook_attacks(occupied, sq) | bishop_attacks(occupied, sq);
}
