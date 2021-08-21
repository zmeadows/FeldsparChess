import prelude;

import bitboard;
import game;
import init;
import movegen;
import perft;
import print;
import quad;

import attacks.base;
import attacks.util;

import unstd.io;
import unstd.random;

import<cstdlib>;
import<cstdio>;
import<string>;
import<cassert>;

int main()
{
    feldspar_init();

    const Game game =
        *game_from_fen("1r3r2/p1qbnpkp/2np2p1/2p1p3/2N1P3/2PP2P1/P2QNPBP/2R2RK1 w - - 6 20");

    QuadBitboard qbb = pack(set_bits<a1, h1, a8, h8>());
    print(qbb);

    constexpr auto numbers = comptime_randoms<1000>(123454345);

    auto odd_count = 0;
    auto even_count = 0;
    for (const U64 x : numbers) {
        to_stdout("{}\n", x);
        if (x % 2 == 0) {
            even_count++;
        } else {
            odd_count++;
        }
    }

    to_stdout("odd = {}, even = {}", odd_count, even_count);

    return 0;
}
