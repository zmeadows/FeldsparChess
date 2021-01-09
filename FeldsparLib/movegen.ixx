export module movegen;

import prelude;
import board;
import game;
import unstd.array;

using MoveBuffer = DynArray<Move, 64>;

export void generate_moves(const Game& game, MoveBuffer& moves) { moves.clear(); }
