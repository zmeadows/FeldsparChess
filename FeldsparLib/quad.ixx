export module quad;

#include "unstd/macros.h"

import prelude;
import bitboard;
import board;
import unstd.io;

import<immintrin.h>;
import<string>;

export using QuadBitboard = __m256i;

export __ALWAYS_INLINE QuadBitboard pack(Bitboard b1, Bitboard b2, Bitboard b3, Bitboard b4)
{
    return _mm256_set_epi64x(b4, b3, b2, b1);
}

export __ALWAYS_INLINE QuadBitboard pack(Bitboard bb) { return _mm256_set1_epi64x(bb); }

export __ALWAYS_INLINE void unpack(QuadBitboard qbb, Bitboard* bbs)
{
    return _mm256_store_si256(reinterpret_cast<QuadBitboard*>(bbs), qbb);
}

export __ALWAYS_INLINE QuadBitboard operator&(const QuadBitboard qbb1, const QuadBitboard qbb2)
{
    return _mm256_and_si256(qbb1, qbb2);
}

export __ALWAYS_INLINE QuadBitboard operator<<(const QuadBitboard qbb1, const QuadBitboard qbb2)
{
    return _mm256_sllv_epi64(qbb1, qbb2);
}

export __ALWAYS_INLINE QuadBitboard operator>>(const QuadBitboard qbb1, const QuadBitboard qbb2)
{
    return _mm256_srlv_epi64(qbb1, qbb2);
}

export __ALWAYS_INLINE QuadBitboard& operator|=(QuadBitboard& qbb1, const QuadBitboard qbb2)
{
    qbb1 = _mm256_or_si256(qbb1, qbb2);
    return qbb1;
}

export __ALWAYS_INLINE Bitboard reduceOR(QuadBitboard x)
{
    alignas(alignof(QuadBitboard)) Bitboard bbs[4];
    unpack(x, bbs);
    return bbs[0] | bbs[1] | bbs[2] | bbs[3];
}

export void print(QuadBitboard qbb)
{
    alignas(alignof(QuadBitboard)) Bitboard bbs[4];
    unpack(qbb, bbs);

    bool bits[4][64] = {false};

    for (size_t i = 0; i < 4; i++) {
        serialize(bbs[i], [&](Square sq) { bits[i][63 - sq] = true; });
    }

    std::string output;
    output.reserve(8 * 4 * 8 + 4 * 8 + 8 + 1);

    for (size_t i = 0; i < 8; i++) {
        for (size_t k = 0; k < 4; k++) {
            for (size_t j = 0; j < 8; j++) {
                output.push_back(bits[k][8*i+j] ? '1' : '0');
            }
            output.push_back(' ');
        }
        output.push_back('\n');
    }

    to_stdout("{}\n", output);
}
