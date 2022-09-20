export module attacks;
#include "unstd/macros.h"

import prelude;

export import attacks.base;
import attacks.classical;
import attacks.pext;


export enum class SliderAlgo {
    Classical,
    PEXT
};

constexpr SliderAlgo DEFAULT_SA = SliderAlgo::PEXT;

export template <SliderAlgo SA = DEFAULT_SA>
__ALWAYS_INLINE Bitboard get_bishop_attacks(Square sq, Bitboard occupied)
{
    if constexpr (SA == SliderAlgo::Classical) {
        return get_bishop_attacks_classical(sq, occupied);
    } else if constexpr (SA == SliderAlgo::PEXT) {
        return get_bishop_attacks_pext(sq, occupied);
    }
}

export template <SliderAlgo SA = DEFAULT_SA>
__ALWAYS_INLINE Bitboard get_rook_attacks(Square sq, Bitboard occupied)
{
    if constexpr (SA == SliderAlgo::Classical) {
        return get_rook_attacks_classical(sq, occupied);
    } else if constexpr (SA == SliderAlgo::PEXT) {
        return get_rook_attacks_pext(sq, occupied);
    }
}

export template <SliderAlgo SA = DEFAULT_SA>
__ALWAYS_INLINE Bitboard get_queen_attacks(Square sq, Bitboard occupied)
{
    return get_bishop_attacks<SA>(sq, occupied) | get_rook_attacks<SA>(sq, occupied);
}

export template <SliderAlgo SA = DEFAULT_SA>
__ALWAYS_INLINE Bitboard xray_rook_attacks(Bitboard occ, Bitboard blockers, Square rook_square)
{
    Bitboard attacks = get_rook_attacks<SA>(rook_square, occ);
    blockers &= attacks;
    return attacks ^ get_rook_attacks<SA>(rook_square, occ ^ blockers);
}

export template <SliderAlgo SA = DEFAULT_SA>
__ALWAYS_INLINE Bitboard xray_bishop_attacks(Bitboard occ, Bitboard blockers, Square bishop_square)
{
    Bitboard attacks = get_bishop_attacks<SA>(bishop_square, occ);
    blockers &= attacks;
    return attacks ^ get_bishop_attacks<SA>(bishop_square, occ ^ blockers);
}
