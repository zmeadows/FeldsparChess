export module move;

import prelude;
import bitboard;
import board;
import game;
import zobrist;

import<string>;
import<cassert>;
import<optional>;

export class MoveBuffer {
    Move moves[255];
    U32 m_count = 0;

public:
    constexpr void append(Move m) { moves[m_count++] = m; }
    constexpr U32 length() const { return m_count; }
    constexpr void clear() { m_count = 0; }

    constexpr const Move* const begin() const { return &moves[0]; }
    constexpr const Move* const end() const { return &moves[0] + m_count; }
};

export inline constexpr U32 QUIET_FLAG = 0b0000;
export inline constexpr U32 DOUBLE_PAWN_PUSH_FLAG = 0b0001;
export inline constexpr U32 KING_CASTLE_FLAG = 0b0010;
export inline constexpr U32 QUEEN_CASTLE_FLAG = 0b0011;
export inline constexpr U32 CAPTURE_FLAG = 0b0100;
export inline constexpr U32 EP_CAPTURE_FLAG = 0b0101;
export inline constexpr U32 KNIGHT_PROMO_FLAG = 0b1000;
export inline constexpr U32 BISHOP_PROMO_FLAG = 0b1001;
export inline constexpr U32 ROOK_PROMO_FLAG = 0b1010;
export inline constexpr U32 QUEEN_PROMO_FLAG = 0b1011;
export inline constexpr U32 KNIGHT_PROMO_CAPTURE_FLAG = 0b1100;
export inline constexpr U32 BISHOP_PROMO_CAPTURE_FLAG = 0b1101;
export inline constexpr U32 ROOK_PROMO_CAPTURE_FLAG = 0b1110;
export inline constexpr U32 QUEEN_PROMO_CAPTURE_FLAG = 0b1111;

export inline constexpr Move NULL_MOVE = 0x0;

// TODO: check if these const intermediate variables are stored in registers or optimized out
// TODO: add assert statements to validate these function inputs
export constexpr Move create_quiet_move(Square from, Square to, U32 flag, PieceType moved_ptype)
{
    const U32 to_bits = static_cast<U32>(to);
    const U32 from_bits = (static_cast<U32>(from) << 6);
    const U32 flag_bits = flag << 12;
    const U32 ptype_bits = static_cast<U32>(moved_ptype) << 16;

    return ptype_bits | flag_bits | from_bits | to_bits;
}

// TODO: add assert statements to validate these function inputs
export constexpr Move create_capture_move(Square from, Square to, U32 flag, PieceType moved_ptype,
                                          PieceType captured_ptype)
{
    const U32 to_bits = static_cast<U32>(to);
    const U32 from_bits = (static_cast<U32>(from) << 6);
    const U32 flag_bits = flag << 12;
    const U32 moved_ptype_bits = static_cast<U32>(moved_ptype) << 16;
    const U32 captured_ptype_bits = static_cast<U32>(captured_ptype) << 19;

    return captured_ptype_bits | moved_ptype_bits | flag_bits | from_bits | to_bits;
}

// TODO: add tests for all of these
export constexpr U32 move_flag(Move move) { return (move >> 12) & 0xF; }

export constexpr bool move_is_capture(Move move) { return (move_flag(move) & 0b0100) != 0; }

export constexpr bool move_is_pawn_promotion(Move move) { return (move_flag(move) & 0b1000) != 0; }

export constexpr Square move_from_square(Move move) { return (move >> 6) & 0x3F; }

export constexpr Square move_to_square(Move move) { return move & 0x3F; }

export constexpr PieceType moved_piece_type(Move move)
{
    return static_cast<PieceType>((move >> 16) & 0x7);
}

// TODO: this will be equal to PieceType::Pawn if the move is not a capture. How bad is that...?
export constexpr PieceType captured_piece_type(Move move)
{
    // assert(move_is_capture(move) && "Called captured_piece_type on non-capture move.");
    return static_cast<PieceType>((move >> 19) & 0x7);
}

// TODO: convert bool template parameters to named enums to avoid mixups
template <Color MOVING_COLOR, bool UPDATE_HASH>
__forceinline void make_move_internal(Game& game, Move move)
{
    using enum PieceType;

    // TODO: only define these constants at the scope required
    const U32 flag = move_flag(move);
    const Square from_sq = move_from_square(move);
    const Square to_sq = move_to_square(move);

    const Bitboard from_bit = square_bitrep(from_sq);
    const Bitboard to_bit = square_bitrep(to_sq);
    const Bitboard from_to_bit = from_bit | to_bit;

    const PieceType moved_ptype = moved_piece_type(move);

    const auto cancel_castling_rights = [&](CastlingRights to_remove) {
        if constexpr (UPDATE_HASH) {
            hash_update_castling_rights(game.hash, game.castling_rights);
        }

        remove_castling_rights(game.castling_rights, to_remove);

        if constexpr (UPDATE_HASH) {
            hash_update_castling_rights(game.hash, game.castling_rights);
        }
    };

    if constexpr (UPDATE_HASH) {
        hash_update_piece_square(game.hash, MOVING_COLOR, moved_ptype, from_sq);
        hash_update_piece_square(game.hash, MOVING_COLOR, moved_ptype, to_sq);
    }

    get_pieces_mut(game.board, moved_ptype, MOVING_COLOR) ^= from_to_bit;
    get_occupied_mut(game.board, MOVING_COLOR) ^= from_to_bit;

    const bool is_capture = flag & 0b0100;
    if (is_capture) [[unlikely]] {
        const PieceType captured_ptype = captured_piece_type(move);

        Bitboard captured_bit;
        Square captured_sq;

        if (flag != EP_CAPTURE_FLAG) [[likely]] {
            captured_bit = to_bit;
            captured_sq = to_sq;
        } else {
            assert(game.ep_square.has_value() &&
                   "En Passant capture move attempted when Game has no ep_square.");

            if constexpr (MOVING_COLOR == Color::White) {
                captured_sq = (*game.ep_square) - 8;
            } else { // MOVING_COLOR == Color::Black
                captured_sq = (*game.ep_square) + 8;
            }
            captured_bit = square_bitrep(captured_sq);
        }

        get_pieces_mut(game.board, captured_ptype, !MOVING_COLOR) ^= captured_bit;
        get_occupied_mut(game.board, !MOVING_COLOR) ^= captured_bit;
        if constexpr (UPDATE_HASH) {
            hash_update_piece_square(game.hash, !MOVING_COLOR, captured_ptype, captured_sq);
        }

        // remove castling rights if rook captured
        if (captured_ptype == Rook) [[unlikely]] {
            if constexpr (MOVING_COLOR == Color::White) {
                if (to_sq == 56) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_BLACK_KINGSIDE);
                } else if (to_sq == 63) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_BLACK_QUEENSIDE);
                }
            } else { // MOVING_COLOR == Color::Black
                if (to_sq == 0) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_WHITE_KINGSIDE);
                } else if (to_sq == 7) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_WHITE_QUEENSIDE);
                }
            }
        }
    }

    // handle special cases, e.g. castling
    switch (moved_ptype)
    {
        case Pawn: {
            if (flag == DOUBLE_PAWN_PUSH_FLAG) {
                if constexpr (UPDATE_HASH) {
                    if (game.ep_square.has_value()) {
                        hash_update_ep_square(game.hash, *game.ep_square);
                    }
                }

                if constexpr (MOVING_COLOR == Color::White) {
                    game.ep_square = to_sq - 8;
                } else { // MOVING_COLOR == Color::Black
                    game.ep_square = to_sq + 8;
                }

                if constexpr (UPDATE_HASH) {
                    hash_update_ep_square(game.hash, *game.ep_square);
                }
            } else if (move_is_pawn_promotion(move)) [[unlikely]] {
                get_pieces_mut(game.board, Pawn, MOVING_COLOR) ^= to_bit;
                if constexpr (UPDATE_HASH) {
                    hash_update_piece_square(game.hash, MOVING_COLOR, Pawn, to_sq);
                }

                if (flag == KNIGHT_PROMO_FLAG || flag == KNIGHT_PROMO_CAPTURE_FLAG) {
                    get_pieces_mut(game.board, Knight, MOVING_COLOR) |= to_bit;
                    if constexpr (UPDATE_HASH) {
                        hash_update_piece_square(game.hash, MOVING_COLOR, Knight, to_sq);
                    }
                } else if (flag == BISHOP_PROMO_FLAG || flag == BISHOP_PROMO_CAPTURE_FLAG) {
                    get_pieces_mut(game.board, Bishop, MOVING_COLOR) |= to_bit;
                    if constexpr (UPDATE_HASH) {
                        hash_update_piece_square(game.hash, MOVING_COLOR, Bishop, to_sq);
                    }
                } else if (flag == ROOK_PROMO_FLAG || flag == ROOK_PROMO_CAPTURE_FLAG) {
                    get_pieces_mut(game.board, Rook, MOVING_COLOR) |= to_bit;
                    if constexpr (UPDATE_HASH) {
                        hash_update_piece_square(game.hash, MOVING_COLOR, Rook, to_sq);
                    }
                } else if (flag == QUEEN_PROMO_FLAG || flag == QUEEN_PROMO_CAPTURE_FLAG) {
                    get_pieces_mut(game.board, Queen, MOVING_COLOR) |= to_bit;
                    if constexpr (UPDATE_HASH) {
                        hash_update_piece_square(game.hash, MOVING_COLOR, Queen, to_sq);
                    }
                }
            }

            break;
        }

        case Rook: {
            if constexpr (MOVING_COLOR == Color::White) {
                if (from_sq == h1) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_WHITE_KINGSIDE);
                } else if (from_sq == a1) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_WHITE_QUEENSIDE);
                }
            } else { // MOVING_COLOR == Color::Black
                if (from_sq == h8) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_BLACK_KINGSIDE);
                } else if (from_sq == a8) [[unlikely]] {
                    cancel_castling_rights(CASTLE_RIGHTS_BLACK_QUEENSIDE);
                }
            }

            break;
        }

        case King: {
            if constexpr (MOVING_COLOR == Color::White) {
                cancel_castling_rights(CASTLE_RIGHTS_WHITE_KINGSIDE);
                cancel_castling_rights(CASTLE_RIGHTS_WHITE_QUEENSIDE);
            } else {
                cancel_castling_rights(CASTLE_RIGHTS_BLACK_KINGSIDE);
                cancel_castling_rights(CASTLE_RIGHTS_BLACK_QUEENSIDE);
            }

            if (flag == KING_CASTLE_FLAG || flag == QUEEN_CASTLE_FLAG) [[unlikely]] {
                Square rook_from_sq, rook_to_sq;

                if constexpr (MOVING_COLOR == Color::White) {
                    if (flag == KING_CASTLE_FLAG) [[unlikely]] {
                        assert(from_sq == e1);
                        rook_from_sq = h1;
                        rook_to_sq = f1;
                    } else if (flag == QUEEN_CASTLE_FLAG) [[unlikely]] {
                        assert(from_sq == e1);
                        rook_from_sq = a1;
                        rook_to_sq = d1;
                    }
                } else {
                    if (flag == KING_CASTLE_FLAG) [[unlikely]] {
                        assert(from_sq == e8);
                        rook_from_sq = h8;
                        rook_to_sq = f8;
                    } else if (flag == QUEEN_CASTLE_FLAG) [[unlikely]] {
                        assert(from_sq == e8);
                        rook_from_sq = a8;
                        rook_to_sq = d8;
                    }
                }

                const Bitboard rook_bit = square_bitrep(rook_from_sq) | square_bitrep(rook_to_sq);

                get_pieces_mut(game.board, Rook, MOVING_COLOR) ^= rook_bit;
                get_occupied_mut(game.board, MOVING_COLOR) ^= rook_bit;

                if constexpr (UPDATE_HASH) {
                    hash_update_piece_square(game.hash, MOVING_COLOR, Rook, rook_from_sq);
                    hash_update_piece_square(game.hash, MOVING_COLOR, Rook, rook_to_sq);
                }
            }

            break;
        }

        default: {
            break;
        }
    }

    if (game.ep_square.has_value() && flag != DOUBLE_PAWN_PUSH_FLAG) {
        if constexpr (UPDATE_HASH) {
            hash_update_ep_square(game.hash, *game.ep_square);
        }
        game.ep_square = {};
    }

    if (is_capture || moved_ptype == Pawn) {
        game.halfmove_clock = 0;
    } else {
        game.halfmove_clock++;
    }

    if constexpr (MOVING_COLOR == Color::Black) {
        game.fullmoves++;
    }

    game.to_move = !game.to_move;
    if constexpr (UPDATE_HASH) {
        hash_update_color_to_move(game.hash);
    }
}

export template <bool UPDATE_HASH>
[[msvc::forceinline_calls]] void make_move(Game& game, Move move)
{
    if (game.to_move == Color::White) {
        make_move_internal<Color::White, UPDATE_HASH>(game, move);
    } else {
        make_move_internal<Color::Black, UPDATE_HASH>(game, move);
    }
}

export std::string move_to_algebraic(Move m)
{
    const Square from = move_from_square(m);
    const Square to = move_to_square(m);

    std::string alg_str;
    alg_str.reserve(4);

    alg_str += square_to_algebraic(from);
    alg_str += square_to_algebraic(to);

    return alg_str;
}

export inline std::optional<Move> move_from_algebraic(const MoveBuffer& moves,
                                                      const std::string& alg)
{
    for (const Move m : moves) {
        if (move_to_algebraic(m) == alg) return m;
    }

    return {};
}
