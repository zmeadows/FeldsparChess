export module attacks.classical;

import prelude;
import bitboard;
import board;
import attacks.rays;

#include "../unstd/macros.h"

// export import attacks.base;

export constexpr __forceinline Bitboard get_bishop_attacks(Square sq, Bitboard occupied)
{
    return get_ray_attacks<Direction::NorthEast>(sq, occupied) |
           get_ray_attacks<Direction::SouthEast>(sq, occupied) |
           get_ray_attacks<Direction::NorthWest>(sq, occupied) |
           get_ray_attacks<Direction::SouthWest>(sq, occupied);
}

export constexpr __forceinline Bitboard get_rook_attacks(Square sq, Bitboard occupied)
{
    return get_ray_attacks<Direction::North>(sq, occupied) |
           get_ray_attacks<Direction::South>(sq, occupied) |
           get_ray_attacks<Direction::East>(sq, occupied) |
           get_ray_attacks<Direction::West>(sq, occupied);
}

export constexpr __forceinline Bitboard get_queen_attacks(Square sq, Bitboard occupied)
{
    return get_bishop_attacks(sq, occupied) | get_rook_attacks(sq, occupied);
}

export constexpr __forceinline Bitboard xray_rook_attacks(Bitboard occ, Bitboard blockers, Square rook_square)
{
    Bitboard attacks = get_rook_attacks(rook_square, occ);
    blockers &= attacks;
    return attacks ^ get_rook_attacks(rook_square, occ ^ blockers);
}

export constexpr __forceinline Bitboard xray_bishop_attacks(Bitboard occ, Bitboard blockers, Square bishop_square)
{
    Bitboard attacks = get_bishop_attacks(bishop_square, occ);
    blockers &= attacks;
    return attacks ^ get_bishop_attacks(bishop_square, occ ^ blockers);
}
