export module movegen;

import prelude;

import bitboard;
import board;
import game;
import move;
import pins;
import print;
import quad;
import unstd.io;

import<cstring>;
import<type_traits>;

#include "unstd/macros.h"

import attacks.classical;
import attacks.util;

export struct MoveGenFlags {
    bool CAPTURES_ONLY = false;
    bool DEBUG_PRINT = false;
};

template <Color FRIENDLY_COLOR, MoveGenFlags FLAGS>
__FLATTEN_CALLS void generate_moves_internal(const Game& game, MoveBuffer& moves)
{
    using enum PieceType;

    moves.clear();

    constexpr Color OPPONENT_COLOR = !FRIENDLY_COLOR;

    const Board& board = game.board;

    const Bitboard friendly_pieces = get_occupied(board, FRIENDLY_COLOR);
    const Bitboard opponent_pieces = get_occupied(board, OPPONENT_COLOR);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_pieces);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, opponent_pieces);

    const Bitboard occupied_squares = friendly_pieces | opponent_pieces;
    const Bitboard empty_squares = bitboard_flipped(occupied_squares);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, occupied_squares);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, empty_squares);

    const Square king_square = bitboard_bsf(get_pieces(board, King, FRIENDLY_COLOR));
    const Bitboard king_attackers = attackers(board, OPPONENT_COLOR, king_square);
    const U64 check_multiplicity = bitboard_popcount(king_attackers);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, king_attackers);

    const auto opponent_piece_type_at = [&](Square sq) -> PieceType {
        const Bitboard sqbit = square_bitrep(sq);

        for (PieceType ptype : EnumRange<PieceType>()) {
            if (sqbit & get_pieces(board, ptype, OPPONENT_COLOR)) [[unlikely]] {
                return ptype;
            }
        }

        assert(false && "Failed to associate square to piece type. Likely invalid capture square.");
        __assume(0);
    };

    Bitboard capture_mask = opponent_pieces;
    Bitboard quiet_mask = empty_squares;

    const auto build_moves = [&](Square from, Bitboard to_mask, PieceType moved_ptype) {
        if constexpr (!FLAGS.CAPTURES_ONLY) {
            serialize(to_mask & quiet_mask, [&](Square to) {
                moves.append(create_quiet_move(from, to, QUIET_FLAG, moved_ptype));
            });
        }

        serialize(to_mask & capture_mask, [&](Square to) {
            moves.append(create_capture_move(from, to, CAPTURE_FLAG, moved_ptype,
                                             opponent_piece_type_at(to)));
        });
    };

    constexpr AttackedFlags ATTACKED_FLAGS = {.ATTACKING_COLOR = OPPONENT_COLOR,
                                              .REMOVE_KING = true};

    const Bitboard king_danger_squares = attacked<ATTACKED_FLAGS>(board);

    const Bitboard safe_king_moves = get_king_moves(king_square) & ~king_danger_squares;
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, safe_king_moves);
    build_moves(king_square, safe_king_moves, King);

    if (check_multiplicity > 1) [[unlikely]] {
        // If the king is in double check, the only legal moves are king moves.
        // Since we just added those to the move buffer, we can return early.
        return;
    } else if (check_multiplicity == 1) [[unlikely]] {
        capture_mask = king_attackers;
        const Square checker_square = bitboard_bsf(king_attackers);

        if (is_slider(opponent_piece_type_at(checker_square))) {
            // When in check by sliding piece, the only allowed quiet moves for
            // non-King pieces must block the attack on the king. 
            quiet_mask &= ray_between_squares(king_square, checker_square);
        } else {
            quiet_mask = BITBOARD_EMPTY;
        }
    }

    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, quiet_mask);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, capture_mask);

    const Pins pins = Pins::create(board, FRIENDLY_COLOR);
    const Bitboard pinned = pins.diagonal | pins.nondiagonal;
    const Bitboard unpinned = ~pinned;
    const Bitboard pinned_only_nondiagonally = pinned & ~pins.diagonal;
    const Bitboard pinned_only_diagonally = pinned & ~pins.nondiagonal;
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, pinned);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, unpinned);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, pinned_only_diagonally);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, pinned_only_nondiagonally);

    // Knights
    const Bitboard friendly_knights = get_pieces(board, Knight, FRIENDLY_COLOR);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_knights);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_knights & unpinned);

    serialize(friendly_knights & unpinned, [&](Square from) {
        const Bitboard knight_moves = get_knight_moves(from);
        build_moves(from, knight_moves, Knight);
    });

    // Bishops
    const Bitboard friendly_bishops = get_pieces(board, Bishop, FRIENDLY_COLOR);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_bishops);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_bishops & unpinned);

    serialize(friendly_bishops & unpinned, [&](Square from) {
        const Bitboard bishop_moves = get_bishop_attacks(from, occupied_squares);
        build_moves(from, bishop_moves, Bishop);
    });

    serialize(friendly_bishops & pins.diagonal, [&](Square from) {
        const Bitboard bishop_moves =
            get_bishop_attacks(from, occupied_squares) & pins.diagonal_constraints[from];
        build_moves(from, bishop_moves, Bishop);
    });

    // Rooks
    const Bitboard friendly_rooks = get_pieces(board, Rook, FRIENDLY_COLOR);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_rooks);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_rooks & unpinned);

    serialize(friendly_rooks & unpinned, [&](Square from) {
        const Bitboard rook_moves = get_rook_attacks(from, occupied_squares);
        build_moves(from, rook_moves, Rook);
    });

    serialize(friendly_rooks & pins.nondiagonal, [&](Square from) {
        const Bitboard rook_moves =
            get_rook_attacks(from, occupied_squares) & pins.nondiagonal_constraints[from];
        build_moves(from, rook_moves, Rook);
    });

    // Queens
    const Bitboard friendly_queens = get_pieces(board, Queen, FRIENDLY_COLOR);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_queens);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_queens & unpinned);

    serialize(friendly_queens & unpinned, [&](Square from) {
        const Bitboard queen_moves = get_queen_attacks(from, occupied_squares);
        build_moves(from, queen_moves, Queen);
    });

    serialize(friendly_queens & pinned_only_diagonally, [&](Square from) {
        const Bitboard queen_moves =
            get_queen_attacks(from, occupied_squares) & pins.diagonal_constraints[from];
        build_moves(from, queen_moves, Queen);
    });

    serialize(friendly_queens & pinned_only_nondiagonally, [&](Square from) {
        const Bitboard queen_moves =
            get_queen_attacks(from, occupied_squares) & pins.nondiagonal_constraints[from];
        build_moves(from, queen_moves, Queen);
    });

    const Bitboard friendly_pawns = get_pieces(board, Pawn, FRIENDLY_COLOR);
    DEBUG_PRINT_BB(FLAGS.DEBUG_PRINT, friendly_pawns);

    Bitboard singly_advanced_pawns, doubly_advanced_pawns, attacking_pawns_mask;
    Square pawn_move_delta;
    S64 promotion_rank;

    { // Unpinned pawns
        if constexpr (FRIENDLY_COLOR == Color::White) {
            pawn_move_delta = 8;
            promotion_rank = 8;

            singly_advanced_pawns =
                empty_squares & bitboard_shifted(friendly_pawns & unpinned, Direction::North);

            doubly_advanced_pawns = bitboard_shifted(singly_advanced_pawns, Direction::North) &
                                    FOURTH_RANK & quiet_mask;

            singly_advanced_pawns &= quiet_mask;

            attacking_pawns_mask = bitboard_shifted(capture_mask, Direction::SouthWest);
            attacking_pawns_mask |= bitboard_shifted(capture_mask, Direction::SouthEast);
            attacking_pawns_mask &= friendly_pawns;
        } else {
            pawn_move_delta = -8;
            promotion_rank = 1;

            singly_advanced_pawns =
                empty_squares & bitboard_shifted(friendly_pawns & unpinned, Direction::South);

            doubly_advanced_pawns =
                bitboard_shifted(singly_advanced_pawns, Direction::South) & FIFTH_RANK & quiet_mask;

            singly_advanced_pawns &= quiet_mask;

            attacking_pawns_mask = bitboard_shifted(capture_mask, Direction::NorthWest);
            attacking_pawns_mask |= bitboard_shifted(capture_mask, Direction::NorthEast);
            attacking_pawns_mask &= friendly_pawns;
        }

        serialize(singly_advanced_pawns, [&](Square to) {
            const Square from = to - pawn_move_delta;
            if (rank_of(to) == promotion_rank) [[unlikely]] {
                moves.append(create_quiet_move(from, to, KNIGHT_PROMO_FLAG, Pawn));
                moves.append(create_quiet_move(from, to, BISHOP_PROMO_FLAG, Pawn));
                moves.append(create_quiet_move(from, to, ROOK_PROMO_FLAG, Pawn));
                moves.append(create_quiet_move(from, to, QUEEN_PROMO_FLAG, Pawn));
            } else [[likely]] {
                moves.append(create_quiet_move(from, to, QUIET_FLAG, Pawn));
            }
        });

        serialize(doubly_advanced_pawns, [&](Square to) {
            const Square from = to - 2 * pawn_move_delta;
            moves.append(create_quiet_move(from, to, DOUBLE_PAWN_PUSH_FLAG, Pawn));
        });
    }

    { // Non-diagonally pinned pawns
        if constexpr (FRIENDLY_COLOR == Color::White) {
            singly_advanced_pawns =
                empty_squares &
                bitboard_shifted(friendly_pawns & pinned_only_nondiagonally, Direction::North);

            doubly_advanced_pawns = bitboard_shifted(singly_advanced_pawns, Direction::North) &
                                    FOURTH_RANK & quiet_mask;

            singly_advanced_pawns &= quiet_mask;
        } else {
            singly_advanced_pawns =
                empty_squares &
                bitboard_shifted(friendly_pawns & pinned_only_nondiagonally, Direction::South);

            doubly_advanced_pawns =
                bitboard_shifted(singly_advanced_pawns, Direction::South) & FIFTH_RANK & quiet_mask;

            singly_advanced_pawns &= quiet_mask;
        }

        serialize(singly_advanced_pawns, [&](Square to) {
            const Square from = to - pawn_move_delta;
            if (square_bitrep(to) & pins.nondiagonal_constraints[from]) {
                moves.append(create_quiet_move(from, to, QUIET_FLAG, Pawn));
            }
        });

        serialize(doubly_advanced_pawns, [&](Square to) {
            const Square from = to - 2 * pawn_move_delta;
            if (square_bitrep(to) & pins.nondiagonal_constraints[from]) {
                moves.append(create_quiet_move(from, to, DOUBLE_PAWN_PUSH_FLAG, Pawn));
            }
        });
    }

    const auto append_pawn_capture = [&](Square from, Square to) {
        if (rank_of(to) == promotion_rank) [[unlikely]] {
            moves.append(create_capture_move(from, to, KNIGHT_PROMO_CAPTURE_FLAG, Pawn,
                                             opponent_piece_type_at(to)));
            moves.append(create_capture_move(from, to, BISHOP_PROMO_CAPTURE_FLAG, Pawn,
                                             opponent_piece_type_at(to)));
            moves.append(create_capture_move(from, to, ROOK_PROMO_CAPTURE_FLAG, Pawn,
                                             opponent_piece_type_at(to)));
            moves.append(create_capture_move(from, to, QUEEN_PROMO_CAPTURE_FLAG, Pawn,
                                             opponent_piece_type_at(to)));
        } else [[likely]] {
            moves.append(
                create_capture_move(from, to, CAPTURE_FLAG, Pawn, opponent_piece_type_at(to)));
        }
    };

    { // fully unpinned pawn attacks
        const Bitboard pawns_that_can_capture = friendly_pawns & unpinned & attacking_pawns_mask;

        serialize(pawns_that_can_capture, [&](Square from) {
            const Bitboard pawn_attack_pattern = get_pawn_attacks(from, FRIENDLY_COLOR);
            serialize(pawn_attack_pattern & capture_mask,
                      [&](Square to) { append_pawn_capture(from, to); });
        });
    }

    { // diagonally pinned pawn attacks
        const Bitboard pawns_that_can_capture =
            friendly_pawns & pinned_only_diagonally & attacking_pawns_mask;

        serialize(pawns_that_can_capture, [&](Square from) {
            const Bitboard pawn_attack_pattern = get_pawn_attacks(from, FRIENDLY_COLOR);
            const Bitboard constraint = pins.diagonal_constraints[from];
            serialize(pawn_attack_pattern & capture_mask, [&](Square to) {
                if (square_bitrep(to) & constraint) [[likely]] {
                    append_pawn_capture(from, to);
                }
            });
        });
    }

    if (game.ep_square.has_value()) [[unlikely]] {
        const Square to = *game.ep_square;
        const Bitboard to_bit = square_bitrep(to);

        Bitboard from_bits;
        if constexpr (FRIENDLY_COLOR == Color::White) {
            from_bits = bitboard_shifted(to_bit, Direction::SouthWest) |
                        bitboard_shifted(to_bit, Direction::SouthEast);
        } else {
            from_bits = bitboard_shifted(to_bit, Direction::NorthWest) |
                        bitboard_shifted(to_bit, Direction::NorthEast);
        }

        serialize(from_bits & friendly_pawns, [&](Square from) {
            assert(is_valid_square(from));

            const Bitboard from_to_bit = square_bitrep(from) | to_bit;

            Bitboard captured_bit;
            if constexpr (FRIENDLY_COLOR == Color::White) {
                captured_bit = square_bitrep(to - 8);
            } else {
                captured_bit = square_bitrep(to + 8);
            }
            assert(captured_bit & get_pieces(game.board, Pawn, OPPONENT_COLOR));

            // An en-passant capture removes two pieces from the board at once,
            // so we cannot rely on our previous pin calculations to account for the
            // rare case in which this move will expose an attack on the player's own king.
            // So we do a mini make_move here and re-calculate attacks on the king.
            Board board_copy;
            memcpy(&board_copy[0], &game.board[0], sizeof(Board));

            get_pieces_mut(board_copy, Pawn, FRIENDLY_COLOR) ^= from_to_bit;
            get_occupied_mut(board_copy, FRIENDLY_COLOR) ^= from_to_bit;
            get_pieces_mut(board_copy, Pawn, OPPONENT_COLOR) ^= captured_bit;
            get_occupied_mut(board_copy, OPPONENT_COLOR) ^= captured_bit;

            const Bitboard king_attackers = attackers(board_copy, OPPONENT_COLOR, king_square);
            if (bitboard_is_empty(king_attackers)) {
                moves.append(create_capture_move(from, to, EP_CAPTURE_FLAG, Pawn, Pawn));
            }
        });
    }

    // Castle Moves
    if constexpr (!(FLAGS.CAPTURES_ONLY)) {
        const bool castle_possible =
            check_multiplicity == 0 &&
            ((FRIENDLY_COLOR == Color::White) ? (game.castling_rights & CASTLE_RIGHTS_WHITE_BOTH)
                                              : (game.castling_rights & CASTLE_RIGHTS_BLACK_BOTH));

        if (castle_possible) [[unlikely]] {
            bool can_kingside_castle;
            bool can_queenside_castle;
            Square king_castle_square, queen_castle_square;

            if constexpr (FRIENDLY_COLOR == Color::White) {
                can_kingside_castle =
                    (game.castling_rights & CASTLE_RIGHTS_WHITE_KINGSIDE) &&
                    bitboard_is_empty(occupied_squares & WHITE_KINGSIDE_CASTLE_PATH) &&
                    bitboard_is_empty(king_danger_squares & WHITE_KINGSIDE_CASTLE_PATH);

                can_queenside_castle =
                    (game.castling_rights & CASTLE_RIGHTS_WHITE_QUEENSIDE) &&
                    bitboard_is_empty(occupied_squares & WHITE_QUEENSIDE_CASTLE_PATH) &&
                    bitboard_is_empty(king_danger_squares & WHITE_QUEENSIDE_CASTLE_SAFETY_REQ);

                king_castle_square = 1;
                queen_castle_square = 5;
            } else {
                can_kingside_castle =
                    (game.castling_rights & CASTLE_RIGHTS_BLACK_KINGSIDE) &&
                    bitboard_is_empty(occupied_squares & BLACK_KINGSIDE_CASTLE_PATH) &&
                    bitboard_is_empty(king_danger_squares & BLACK_KINGSIDE_CASTLE_PATH);

                can_queenside_castle =
                    (game.castling_rights & CASTLE_RIGHTS_BLACK_QUEENSIDE) &&
                    bitboard_is_empty(occupied_squares & BLACK_QUEENSIDE_CASTLE_PATH) &&
                    bitboard_is_empty(king_danger_squares & BLACK_QUEENSIDE_CASTLE_SAFETY_REQ);

                king_castle_square = 57;
                queen_castle_square = 61;
            }

            if (can_kingside_castle) [[unlikely]] {
                moves.append(
                    create_quiet_move(king_square, king_castle_square, KING_CASTLE_FLAG, King));
            }

            if (can_queenside_castle) [[unlikely]] {
                moves.append(
                    create_quiet_move(king_square, queen_castle_square, QUEEN_CASTLE_FLAG, King));
            }
        }
    }
}

export template <MoveGenFlags FLAGS = MoveGenFlags()>
void __ALWAYS_INLINE generate_moves(const Game& game, MoveBuffer& moves)
{
    using enum Color;
    if (game.to_move == White) {
        generate_moves_internal<White, FLAGS>(game, moves);
    } else {
        generate_moves_internal<Black, FLAGS>(game, moves);
    }
}

export void debug_print_movegen(const Game& game)
{
    MoveBuffer moves;
    generate_moves<{.DEBUG_PRINT = true}>(game, moves);

    for (const Move m : moves) {
        print_move(m);
    }
}
