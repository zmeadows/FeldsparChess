export module attacks.classical;
#include "../unstd/macros.h"

import prelude;
import bitboard;
import board;
import attacks.rays;

export constexpr __ALWAYS_INLINE Bitboard get_bishop_attacks_classical(Square sq, Bitboard occupied)
{
    return get_ray_attacks<Direction::NorthEast>(sq, occupied) |
           get_ray_attacks<Direction::SouthEast>(sq, occupied) |
           get_ray_attacks<Direction::NorthWest>(sq, occupied) |
           get_ray_attacks<Direction::SouthWest>(sq, occupied);
}

export constexpr __ALWAYS_INLINE Bitboard get_rook_attacks_classical(Square sq, Bitboard occupied)
{
    return get_ray_attacks<Direction::North>(sq, occupied) |
           get_ray_attacks<Direction::South>(sq, occupied) |
           get_ray_attacks<Direction::East>(sq, occupied) |
           get_ray_attacks<Direction::West>(sq, occupied);
}
