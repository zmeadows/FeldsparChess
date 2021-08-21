export module unstd.random;

import unstd.core;
import<array>;

export template <typename T>
T rand(void)
{
    static_assert(false, "unstd.rand not implemented for type.");
    return T();
}

// random seed values
thread_local U64 s[4] = {0xe066a3beb543235a, 0x616300ea38021090, 0xe239b667a4fd40f3,
                         0x3d77eb67a5bb6a73};

template <>
U64 rand<U64>(void)
{
    constexpr auto rol64 = [](U64 x, int k) -> U64 {
        return (x << k) | (x >> (64 - k));
    };

    const U64 result = rol64(s[1] * 5, 7) * 9;
    const U64 t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;
    s[3] = rol64(s[3], 45);

    return result;
}

export template <typename T>
T rand(const T min, const T max)
{
    static_assert(false, "rand(min,max) not implemented for type.");
    return T();
}

template <>
U64 rand<U64>(const U64 min, const U64 max)
{
    return min + (rand<U64>() % (max - min + 1));
}

// en.wikipedia.org/wiki/Permuted_congruential_generator
export template <size_t N>
constexpr std::array<U64,N> comptime_randoms(U64 seed)
{
    constexpr U64 multiplier = 6364136223846793005ULL;
    constexpr U64 increment = 1442695040888963407ULL; // arbitrary odd constant

    // bit permutation
    constexpr auto rotr32 = [](U32 x, U32 r) -> U32 {
        return x >> r | x << ((~r + 1) & 31);
    };

    U64 state = seed + increment;
    constexpr auto pcg32 = [&]() -> U32 {
        U64 x = state;
        const U32 count = static_cast<U32>(x >> 59); // 59 = 64 - 5
        state = x * multiplier + increment;
        x ^= x >> 18;                              // 18 = (64 - 27)/2
        return rotr32(static_cast<U32>(x >> 27), count); // 27 = 32 - 5
    };

    std::array<U64, N> results;

    // Use OR of two 32-bit random numbers to construct a random 64-bit number.
    for (auto i = 0; i < N; i++) {
        const U64 left32 = static_cast<U64>(pcg32());
        const U64 right32 = static_cast<U64>(pcg32());
        results[i] = (left32 << 32) | right32;
    }

    return results;
}

