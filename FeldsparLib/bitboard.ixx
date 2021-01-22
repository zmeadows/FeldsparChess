export module bitboard;

import prelude;

import<cassert>;
import<cstdio>;
import<intrin.h>;

template <size_t N, size_t I>
constexpr Bitboard BB_helper(const char* input)
{
    if (input[I] != '0' && input[I] != '1') {
        assert(false);
    }

    if constexpr (I == 0) {
        return (static_cast<Bitboard>(input[0]) - '0') << (N);
    }
    else {
        return (static_cast<Bitboard>(input[I]) - '0') << (N - I) | BB_helper<N, I - 1>(input);
    }
}

// Convert a compile-time string representation of a bitboard to a U64
export template <size_t N>
constexpr Bitboard BB(const char (&input)[N])
{
    static_assert(N == 65, "string-to-bitboard converstion (BB) requires binary literal "
                           "string with 64 characters.");

    // Use N-2 to skip past trailing null terminator in string constant
    return BB_helper<N - 2, N - 2>(input);
}

export constexpr Bitboard square_bitrep(Square sq) { return static_cast<Bitboard>(1) << sq; }

export inline constexpr const Bitboard BITBOARD_FULL = 0xFFFFFFFFFFFFFFFF;
export inline constexpr const Bitboard BITBOARD_EMPTY = 0x0;

export inline constexpr const Bitboard H_FILE = BB("00000001"
                                                   "00000001"
                                                   "00000001"
                                                   "00000001"
                                                   "00000001"
                                                   "00000001"
                                                   "00000001"
                                                   "00000001");

export inline constexpr const Bitboard G_FILE = H_FILE << 1;
export inline constexpr const Bitboard F_FILE = H_FILE << 2;
export inline constexpr const Bitboard E_FILE = H_FILE << 3;
export inline constexpr const Bitboard D_FILE = H_FILE << 4;
export inline constexpr const Bitboard C_FILE = H_FILE << 5;
export inline constexpr const Bitboard B_FILE = H_FILE << 6;
export inline constexpr const Bitboard A_FILE = H_FILE << 7;

export inline constexpr const Bitboard NOT_A_FILE = ~A_FILE;
export inline constexpr const Bitboard NOT_B_FILE = ~B_FILE;
export inline constexpr const Bitboard NOT_C_FILE = ~C_FILE;
export inline constexpr const Bitboard NOT_D_FILE = ~D_FILE;
export inline constexpr const Bitboard NOT_E_FILE = ~E_FILE;
export inline constexpr const Bitboard NOT_F_FILE = ~F_FILE;
export inline constexpr const Bitboard NOT_G_FILE = ~G_FILE;
export inline constexpr const Bitboard NOT_H_FILE = ~H_FILE;

export inline constexpr const Bitboard FIRST_RANK = BB("00000000"
                                                       "00000000"
                                                       "00000000"
                                                       "00000000"
                                                       "00000000"
                                                       "00000000"
                                                       "00000000"
                                                       "11111111");

export inline constexpr const Bitboard SECOND_RANK = FIRST_RANK << 8 * 1;
export inline constexpr const Bitboard THIRD_RANK = FIRST_RANK << 8 * 2;
export inline constexpr const Bitboard FOURTH_RANK = FIRST_RANK << 8 * 3;
export inline constexpr const Bitboard FIFTH_RANK = FIRST_RANK << 8 * 4;
export inline constexpr const Bitboard SIXTH_RANK = FIRST_RANK << 8 * 5;
export inline constexpr const Bitboard SEVENTH_RANK = FIRST_RANK << 8 * 6;
export inline constexpr const Bitboard EIGTH_RANK = FIRST_RANK << 8 * 7;

export inline constexpr const Bitboard NOT_FIRST_RANK = ~FIRST_RANK;
export inline constexpr const Bitboard NOT_SECOND_RANK = ~SECOND_RANK;
export inline constexpr const Bitboard NOT_THIRD_RANK = ~THIRD_RANK;
export inline constexpr const Bitboard NOT_FOURTH_RANK = ~FOURTH_RANK;
export inline constexpr const Bitboard NOT_FIFTH_RANK = ~FIFTH_RANK;
export inline constexpr const Bitboard NOT_SIXTH_RANK = ~SIXTH_RANK;
export inline constexpr const Bitboard NOT_SEVENTH_RANK = ~SEVENTH_RANK;
export inline constexpr const Bitboard NOT_EIGTH_RANK = ~EIGTH_RANK;

export inline constexpr const Bitboard MAIN_DIAGONAL = BB("00000001"
                                                          "00000010"
                                                          "00000100"
                                                          "00001000"
                                                          "00010000"
                                                          "00100000"
                                                          "01000000"
                                                          "10000000");

export inline constexpr const Bitboard MAIN_ANTIDIAGONAL = BB("10000000"
                                                              "01000000"
                                                              "00100000"
                                                              "00010000"
                                                              "00001000"
                                                              "00000100"
                                                              "00000010"
                                                              "00000001");

export constexpr bool bitboard_is_empty(const Bitboard bb) { return bb == BITBOARD_EMPTY; }
export constexpr bool bitboard_is_occupied(const Bitboard bb) { return bb != BITBOARD_EMPTY; }
export constexpr bool bitboard_is_full(const Bitboard bb) { return bb == BITBOARD_FULL; }

// TODO: make tests for these
export __forceinline constexpr Bitboard set_bit(Bitboard bb, Square sq)
{
    return bb | (Bitboard(1) << sq);
}
export __forceinline constexpr Bitboard clear_bit(Bitboard bb, Square sq)
{
    return bb & ~(Bitboard(1) << sq);
}
export __forceinline constexpr Bitboard toggle_bit(Bitboard bb, Square sq)
{
    return bb ^ (Bitboard(1) << sq);
}
export __forceinline constexpr bool check_bit(Bitboard bb, Square sq)
{
    return ((bb >> sq) & Bitboard(1)) > 0;
}

export __forceinline constexpr Bitboard bitboard_flipped(Bitboard bb) { return ~bb; }

export __forceinline Square bitboard_bsf(Bitboard bb)
{
    unsigned long idx = 0;
    _BitScanForward64(&idx, bb);
    return static_cast<Square>(idx);
}

export __forceinline Square bitboard_bsr(Bitboard bb)
{
    unsigned long idx = 0;
    _BitScanReverse64(&idx, bb);
    return static_cast<Square>(idx);
}

export __forceinline U64 bitboard_popcount(Bitboard bb) { return __popcnt64(bb); }

export template <typename Callable>
__forceinline void serialize(Bitboard bb, Callable&& f)
{
    while (bb != 0) {
        f(bitboard_bsf(bb));
        bb &= (bb - 1);
    }
}

// Any bits shifted 'off the board' disappear rather than wrap around
export constexpr Bitboard bitboard_shifted(const Bitboard bb, const Direction dir)
{
    switch (dir) {
        case Direction::North:
            return bb << 8;
        case Direction::South:
            return bb >> 8;
        case Direction::East:
            return (bb << 1) & NOT_A_FILE;
        case Direction::West:
            return (bb >> 1) & NOT_H_FILE;
        case Direction::NorthEast:
            return (bb << 7) & NOT_A_FILE;
        case Direction::NorthWest:
            return (bb << 9) & NOT_H_FILE;
        case Direction::SouthEast:
            return (bb >> 7) & NOT_A_FILE;
        case Direction::SouthWest:
            return (bb << 7) & NOT_H_FILE;
        default:
            return 0;
    }
}

export void print_bitboard(Bitboard bb)
{
    bool bits[64] = {0};
    serialize(bb, [&](Square sq) { bits[63 - sq] = true; });

    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            const size_t idx = 8 * i + j;
            printf("%c", (bits[idx] ? '1' : '0'));
        }
        printf("\n");
    }
    printf("\n");
}

export __forceinline constexpr Bitboard rank_mask(Square sq) { return FIRST_RANK << (sq & 56); }
export __forceinline constexpr Bitboard file_mask(Square sq) { return H_FILE << (sq & 7); }
export __forceinline constexpr S64 rank_of(Square sq) { return 1 + sq & 56; }
export __forceinline constexpr S64 file_of(Square sq) { return 8 - sq & 7; }

export inline constexpr Bitboard rank_mask_ex(Square sq)
{
    return square_bitrep(sq) ^ rank_mask(sq);
}

export inline constexpr Bitboard file_mask_ex(Square sq)
{
    return square_bitrep(sq) ^ file_mask(sq);
}

// TODO: use algebra of boolean operators to simplify
export inline constexpr Bitboard diag_mask(Square sq)
{
    sq = (sq & 56) + 7 - (sq & 7);
    const S64 diag = 8 * (sq & 7) - (sq & 56);
    const S64 nort = -diag & (diag >> 31);
    const S64 sout = diag & (-diag >> 31);
    return (MAIN_DIAGONAL >> sout) << nort;
}

// TODO: use algebra of boolean operators to simplify
export inline constexpr Bitboard antidiag_mask(Square sq)
{
    sq = (sq & 56) + 7 - (sq & 7);
    const S64 diag = 56 - 8 * (sq & 7) - (sq & 56);
    const S64 nort = -diag & (diag >> 31);
    const S64 sout = diag & (-diag >> 31);
    return (MAIN_ANTIDIAGONAL >> sout) << nort;
}

export inline constexpr Bitboard diag_mask_ex(Square sq)
{
    return square_bitrep(sq) ^ diag_mask(sq);
}

export inline constexpr Bitboard antidiag_mask_ex(Square sq)
{
    return square_bitrep(sq) ^ antidiag_mask(sq);
}

export inline constexpr Bitboard bishop_mask_ex(Square sq)
{
    return diag_mask_ex(sq) | antidiag_mask_ex(sq);
}

/**
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 */
// export constexpr Square bitboard_bsf_comptime(Bitboard bb)
// {
//     constexpr Square index64[64] = {0,  1,  48, 2,  57, 49, 28, 3,  61, 58, 50, 42, 38, 29, 17,
//     4,
//                                     62, 55, 59, 36, 53, 51, 43, 22, 45, 39, 33, 30, 24, 18, 12,
//                                     5, 63, 47, 56, 27, 60, 41, 37, 16, 54, 35, 52, 21, 44, 32,
//                                     23, 11, 46, 26, 40, 15, 34, 20, 31, 10, 25, 14, 19, 9,  13,
//                                     8,  7,  6};
//
//     constexpr Bitboard debruijn64 = 0x03f79d71b4cb0a89;
//     return index64[((bb & -bb) * debruijn64) >> 58];
// }
