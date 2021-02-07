export module unstd.random;

import unstd.core;

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
