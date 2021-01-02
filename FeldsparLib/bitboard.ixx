export module bitboard;

import prelude;

import<cassert>;
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

export inline constexpr Bitboard BITBOARD_FULL = 0xFFFFFFFFFFFFFFFF;
export inline constexpr Bitboard BITBOARD_EMPTY = 0x0;

export inline constexpr Bitboard NOT_A_FILE = BB("01111111"
                                                 "01111111"
                                                 "01111111"
                                                 "01111111"
                                                 "01111111"
                                                 "01111111"
                                                 "01111111"
                                                 "01111111");

export inline constexpr Bitboard NOT_H_FILE = BB("11111110"
                                                 "11111110"
                                                 "11111110"
                                                 "11111110"
                                                 "11111110"
                                                 "11111110"
                                                 "11111110"
                                                 "11111110");

export constexpr bool bitboard_is_empty(const Bitboard bb) { return bb == BITBOARD_EMPTY; }
export constexpr bool bitboard_is_occupied(const Bitboard bb) { return bb != BITBOARD_EMPTY; }
export constexpr bool bitboard_is_full(const Bitboard bb) { return bb == BITBOARD_FULL; }

export constexpr Bitboard bitboard_flipped(Bitboard bb) { return ~bb; }

export inline Square bitboard_bsf(Bitboard bb)
{
    unsigned long idx = 0;
    _BitScanForward64(&idx, bb);
    return static_cast<Square>(idx);
}

export inline Square bitboard_bsr(Bitboard bb)
{
    unsigned long idx = 0;
    _BitScanReverse64(&idx, bb);
    return static_cast<Square>(idx);
}

export U64 bitboard_popcount(Bitboard bb) { return __popcnt64(bb); }

export template <typename Callable>
inline void bitboard_iter_squares(Bitboard bb, Callable&& f)
{
    while (bb != 0) {
        f(bitboard_bsf(bb));
        bb &= (bb - 1);
    }
}

// export class Serialize {
// 	const Bitboard m_bb;
//
// 	struct Iterator {
// 		Bitboard value;
// 		explicit __forceinline constexpr Iterator(Bitboard b) : value(b) {}
//
// 		constexpr __forceinline void operator++() { value &= (value - 1); }
// 		constexpr __forceinline bool operator!=(Iterator rhs) { return value != rhs.value; }
// 		inline Square operator*() const { return bitboard_bsf(value); }
// 	};
//
// 	Serialize(void) = delete;
//
// public:
// 	constexpr __forceinline Serialize(Bitboard b) : m_bb(b) {};
// 	constexpr __forceinline Iterator begin() const { return Iterator(m_bb); }
// 	constexpr __forceinline Iterator end() const {
// 		return Iterator(BITBOARD_EMPTY);
// 	}
// };

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

// export void print_bitboard(Bitboard bb, std::ostream& stream = std::cout)
// {
// 	bool bits[64] = { 0 };
// 	bitboard_iter_squares(bb, [&](Square sq) { bits[63 - sq] = true; });
//
// 	for (size_t i = 0; i < 8; i++) {
// 		for (size_t j = 0; j < 8; j++) {
// 			const size_t idx = 8 * i + j;
// 			stream << (bits[idx] ? '1' : '0');
// 		}
// 		stream << ' ' << std::endl;
// 	}
// 	stream << std::endl;
// }
