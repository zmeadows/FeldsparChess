export module movegen;

import bitboard;
import board;
import game;
import pins;
import prelude;
import rays;

import unstd.array;

import<cassert>;

using MoveBuffer = DynArray<Move, 256>;

export template <bool CAPTURES_ONLY>
void generate_moves(const Game& game, MoveBuffer& moves)
{
    moves.clear();

    const Color friendly_color = game.to_move;
    const Pins pins = Pins::create(game.board, friendly_color);

    const Color opponent_color = !friendly_color;
    const Bitboard friendly_pieces = get_occupied(game.board, friendly_color);
    const Bitboard opponent_pieces = get_occupied(game.board, !friendly_color);
    const Bitboard occupied_squares = friendly_pieces | opponent_pieces;
}
