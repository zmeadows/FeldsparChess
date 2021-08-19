export module unstd.random;

import unstd.core;
import<array>;

export template <typename T>
T rand()
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
    constexpr auto rol64 = [](U64 x, int k) -> U64 { return (x << k) | (x >> (64 - k)); };

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
constexpr std::array<U64,N> crand(U64 seed) {
    std::array<U64, N> numbers;

    constexpr U64 multiplier = 6364136223846793005u;
    constexpr U64 increment = 1442695040888963407u; // arbitrary odd constant
    U64 state = seed + increment;

    constexpr auto rotr32 = [](uint32_t x, unsigned r) -> U32 { return x >> r | x << (-r & 31); };

    constexpr auto pcg32 = [&]() -> U32 {
        uint64_t x = state;
        unsigned count = (unsigned)(x >> 59); // 59 = 64 - 5

        state = x * multiplier + increment;
        x ^= x >> 18;                              // 18 = (64 - 27)/2
        return rotr32((uint32_t)(x >> 27), count); // 27 = 32 - 5
    };

    // Use combination of two 32-bit random numbers to create a random 64-bit number.
    for (auto i = 0; i < N; i++) {
        const U64 msbs = static_cast<U64>(pcg32());
        const U64 lsbs = static_cast<U64>(pcg32());
        numbers[i] = (msbs << 32) | lsbs;
    }

    return numbers;
}

