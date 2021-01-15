export module quad;

import prelude;
import bitboard;

import<immintrin.h>;

export using QuadBitboard = __m256i;

// export __forceinline Bitboard reduceOR(QuadBitboard qbb) {}

// TODO: can flipping function arguments here optimize the assembly? unlikely...
export __forceinline QuadBitboard pack(Bitboard b1, Bitboard b2, Bitboard b3, Bitboard b4)
{
    return _mm256_set_epi64x(b4, b3, b2, b1);
}

export __forceinline QuadBitboard pack(Bitboard bb) { return _mm256_set1_epi64x(bb); }

export __forceinline void unpack(QuadBitboard qbb, Bitboard* bbs)
{
    return _mm256_store_si256(reinterpret_cast<QuadBitboard*>(bbs), qbb);
}

export __forceinline QuadBitboard operator&(const QuadBitboard qbb1, const QuadBitboard qbb2)
{
    return _mm256_and_si256(qbb1, qbb2);
}

export __forceinline QuadBitboard operator<<(const QuadBitboard qbb1, const QuadBitboard qbb2)
{
    return _mm256_sllv_epi64(qbb1, qbb2);
}

export __forceinline QuadBitboard operator>>(const QuadBitboard qbb1, const QuadBitboard qbb2)
{
    return _mm256_srlv_epi64(qbb1, qbb2);
}

export __forceinline QuadBitboard& operator|=(QuadBitboard& qbb1, QuadBitboard qbb2)
{
    qbb1 = _mm256_or_si256(qbb1, qbb2);
    return qbb1;
}

// qsliders = {rq,rq,bq,bq}
export QuadBitboard east_nort_noWe_noEa_Attacks(QuadBitboard qsliders, Bitboard empty)
{
    const QuadBitboard qmask = pack(NOT_A_FILE, -1, NOT_H_FILE, NOT_A_FILE);
    const QuadBitboard qshift = pack(1, 8, 7, 9);
    QuadBitboard qflood = qsliders;
    QuadBitboard qempty = pack(empty) & qmask;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= qsliders = (qsliders << qshift) & qempty;
    qflood |= (qsliders << qshift) & qempty;
    return (qflood << qshift) & qmask;
}

// qsliders = {rq,rq,bq,bq}
QuadBitboard west_sout_soEa_soWe_Attacks(QuadBitboard qsliders, Bitboard empty)
{
    const QuadBitboard qmask = pack(NOT_H_FILE, -1, NOT_A_FILE, NOT_H_FILE);
    const QuadBitboard qshift = pack(1, 8, 7, 9);
    QuadBitboard qflood = qsliders;
    QuadBitboard qempty = pack(empty) & qmask;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= qsliders = (qsliders >> qshift) & qempty;
    qflood |= (qsliders >> qshift) & qempty;
    return (qflood >> qshift) & qmask;
}
