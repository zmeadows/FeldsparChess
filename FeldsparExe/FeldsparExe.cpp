import game;
import move;
import movegen;
import zobrist;

import<cstdio>;

int main()
{
    init_zobrist_hashing();

    const Game og = *Game::create("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    MoveBuffer moves;
    while (true) {
        generate_moves<false, false>(og, moves);
    }

    printf("%llu\n", moves.length());

    return 0;
}
