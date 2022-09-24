export module print;

import prelude;
import unstd.io;

import bitboard;
import board;
import game;
import move;

export void print_move(Move m)
{
    const char* from_alg = square_to_algebraic(move_from_square(m));
    const char* to_alg = square_to_algebraic(move_to_square(m));
    to_stdout("{} -> {}\n", from_alg, to_alg);
}

export void print_bitboard(Bitboard bb)
{
    bool bits[64] = {0};
    serialize(bb, [&](Square sq) { bits[63 - sq] = true; });

    std::string output;
    output.reserve(8 * 8 + 8 + 1);

    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            const size_t idx = 8 * i + j;
            output.push_back(bits[idx] ? '1' : '0');
        }
        output.push_back('\n');
    }
    output.push_back('\n');

    to_stdout(output);
}

export void print_board(const Board& board) {}

export void print_game(const Game& board) {}

export void print_game_detailed(const Game& board) {}
