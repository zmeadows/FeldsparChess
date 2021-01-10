export module quad;

import prelude;

import<immintrin.h>;

class QuadBitboard {
    __m256i vec;

public:
    QuadBitboard(Bitboard b1, Bitboard b2, Bitboard b3, Bitboard b4)
        : vec(_mm256_set_epi64x(b1, b2, b3, b4))
    {
    }
};

// qsliders = {rq,rq,bq,bq}
// QuadBitboard east_nort_noWe_noEa_Attacks(QuadBitboard qsliders, Bitboard empty)
// {
//     const QuadBitboard qmask(NOT_A_FILE, -1, NOT_H_FILE, NOT_A_FILE);
//     const QuadBitboard qshift(1, 8, 7, 9);
//     QuadBitboard qflood(sliders);
//     QuadBitboard qempty = QuadBitboard(empty) & qmask;
//     qflood |= qsliders = (qsliders << qshift) & qempty;
//     qflood |= qsliders = (qsliders << qshift) & qempty;
//     qflood |= qsliders = (qsliders << qshift) & qempty;
//     qflood |= qsliders = (qsliders << qshift) & qempty;
//     qflood |= qsliders = (qsliders << qshift) & qempty;
//     qflood |= (qsliders << qshift) & qempty;
//     return (qflood << qshift) & qmask
// }
