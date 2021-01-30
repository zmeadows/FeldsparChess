export module print;

import bitboard;
import board;
import game;
import move;
import prelude;

import<cstdio>;

export void print_move(Move m)
{
    StringRef from_alg = square_to_algebraic(move_from_square(m));
    StringRef to_alg = square_to_algebraic(move_to_square(m));
    printf("%s -> %s\n", from_alg.cstr(), to_alg.cstr());
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

export void print_board(const Board& board) {}

export void print_game(const Game& board) {}

export void print_game_detailed(const Game& board) {}
