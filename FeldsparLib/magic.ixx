export module magic;

import unstd.core;
import unstd.array;

import prelude;
import bitboard;

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
