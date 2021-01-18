export module move;

import prelude;
import bitboard;
import board;
import game;
import zobrist;

import<type_traits>;
static_assert(std::is_same<Move, U32>::value, "ALERT! Move underlying type changed!");

export class MoveBuffer {
    Move moves[220];
    U64 m_count = 0;

public:
    __forceinline U64 length() const { return m_count; }

    __forceinline void clear() { m_count = 0; }

    __forceinline void append(Move m)
    {
        moves[m_count] = m;
        m_count++;
    }
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
export __forceinline constexpr Move create_quiet_move(Square from, Square to, U32 flag,
                                                      PieceType moved_ptype)
{
    const U32 to_bits = static_cast<U32>(to);
    const U32 from_bits = (static_cast<U32>(from) << 6);
    const U32 flag_bits = flag << 12;
    const U32 ptype_bits = static_cast<U32>(moved_ptype) << 16;

    return ptype_bits | flag_bits | from_bits | to_bits;
}

export __forceinline constexpr Move create_capture_move(Square from, Square to, U32 flag,
                                                        PieceType moved_ptype,
                                                        PieceType captured_ptype)
{
    const U32 to_bits = static_cast<U32>(to);
    const U32 from_bits = (static_cast<U32>(from) << 6);
    const U32 flag_bits = flag << 12;
    const U32 moved_ptype_bits = static_cast<U32>(moved_ptype) << 16;
    const U32 captured_ptype_bits = static_cast<U32>(captured_ptype) << 19;

    return captured_ptype_bits | moved_ptype_bits | flag_bits | from_bits | to_bits;
}

__forceinline constexpr bool move_is_capture(Move move) { return (move & (1ULL << 15)) != 0; }
__forceinline constexpr bool move_is_pawn_promotion(Move move) { return 0; }
__forceinline constexpr bool move_from_square(Move move) { return 0; }
__forceinline constexpr bool move_to_square(Move move) { return 0; }
__forceinline constexpr PieceType moved_piece_type(Move move) { return PieceType::Pawn; }
__forceinline constexpr PieceType captured_piece_type(Move move) { return PieceType::Pawn; }
__forceinline constexpr U32 move_flag(Move move) { return 0UL; }

template <Color MOVING_COLOR>
void make_move_internal(Game& game, Move move)
{
    using enum PieceType;

    const U32 flag = move_flag(move);
    const Square from_sq = move_from_square(move);
    const Square to_sq = move_to_square(move);

    const Bitboard from_bit = square_bitrep(from_sq);
    const Bitboard to_bit = square_bitrep(to_sq);
    const Bitboard from_to_bit = from_bit | to_bit;

    const bool is_capture = move_is_capture(move);
    const PieceType moved_ptype = moved_piece_type(move);
    const PieceType captured_ptype = captured_piece_type(move);

    constexpr CastlingRights REMOVE_WHITE_KINGSIDE = ~WHITE_KINGSIDE;
    constexpr CastlingRights REMOVE_WHITE_QUEENSIDE = ~WHITE_QUEENSIDE;
    constexpr CastlingRights REMOVE_BLACK_KINGSIDE = ~BLACK_KINGSIDE;
    constexpr CastlingRights REMOVE_BLACK_QUEENSIDE = ~BLACK_QUEENSIDE;

    const auto remove_castling_rights = [&](CastlingRights castle_mask) {
        hash_update_castling_rights(game.hash, game.castling_rights);
        game.castling_rights &= castle_mask;
        hash_update_castling_rights(game.hash, game.castling_rights);
    };

    hash_update_piece_square(game.hash, MOVING_COLOR, moved_ptype, from_sq);
    hash_update_piece_square(game.hash, MOVING_COLOR, moved_ptype, to_sq);
    get_pieces_mut(game.board, moved_ptype, MOVING_COLOR) ^= from_to_bit;
    get_occupied_mut(game.board, MOVING_COLOR) ^= from_to_bit;

    if (is_capture) {
        if (captured_ptype == Rook) [[unlikely]] {
            if constexpr (MOVING_COLOR == Color::White) {
                if ((to_sq == 56) && (game.castling_rights & BLACK_KINGSIDE)) [[unlikely]] {
                    remove_castling_rights(REMOVE_BLACK_KINGSIDE);
                }
                else if ((to_sq == 63) && (game.castling_rights & BLACK_QUEENSIDE)) [[unlikely]] {
                    remove_castling_rights(REMOVE_BLACK_QUEENSIDE);
                }
            }
            else { // MOVING_COLOR == Color::Black
                if ((to_sq == 0) && (game.castling_rights & WHITE_KINGSIDE)) [[unlikely]] {
                    remove_castling_rights(REMOVE_WHITE_KINGSIDE);
                }
                else if ((to_sq == 7) && (game.castling_rights & WHITE_QUEENSIDE)) [[unlikely]] {
                    remove_castling_rights(REMOVE_WHITE_QUEENSIDE);
                }
            }
        }

        if (moved_ptype != Pawn) {
            // Pawn captures are handled separately for promotions and en passant logic

            // *self.board.get_pieces_mut(opponent_color, captured_ptype.unwrap()) ^= to_bit;
            // *self.board.occupied_by_mut(opponent_color) ^= to_bit;
            // self.hash.change_piece(opponent_color, captured_ptype.unwrap(), to_sq);
        }
    }

    switch (moved_ptype) {
        case Pawn: {
            if (flag == DOUBLE_PAWN_PUSH_FLAG) [[unlikely]] {
            }
            else if (is_capture) [[unlikely]] {
                if (flag == EP_CAPTURE_FLAG) [[unlikely]] {
                }
                else [[likely]] {
                }
            }

            if (move_is_pawn_promotion(move)) [[unlikely]] {
            }
            break;
        }
        case Rook: {
            if constexpr (MOVING_COLOR == Color::White) {
                if (from_sq == 0 && (game.castling_rights & WHITE_KINGSIDE)) [[unlikely]] {
                    remove_castling_rights(REMOVE_WHITE_KINGSIDE);
                }
                else if (from_sq == 7 && (game.castling_rights & WHITE_QUEENSIDE)) [[unlikely]] {
                    remove_castling_rights(REMOVE_WHITE_QUEENSIDE);
                }
            }
            else { // MOVING_COLOR == Color::Black
                if (from_sq == 56) [[unlikely]] {
                    remove_castling_rights(REMOVE_BLACK_KINGSIDE);
                }
                else if (from_sq == 63) [[unlikely]] {
                    remove_castling_rights(REMOVE_BLACK_QUEENSIDE);
                }
            }
            break;
        }
        case King: {
            if constexpr (MOVING_COLOR == Color::White) {
                if (flag == KING_CASTLE_FLAG) [[unlikely]] {
                }
                else if (flag == QUEEN_CASTLE_FLAG) [[unlikely]] {
                }

                remove_castling_rights(REMOVE_WHITE_KINGSIDE & REMOVE_WHITE_QUEENSIDE);
            }
            else { // MOVING_COLOR == Color::Black
                if (flag == KING_CASTLE_FLAG) [[unlikely]] {
                }
                else if (flag == QUEEN_CASTLE_FLAG) [[unlikely]] {
                }

                remove_castling_rights(REMOVE_BLACK_KINGSIDE & REMOVE_BLACK_QUEENSIDE);
            }
            break;
        }
        default: {
            break;
        }
    }

    if (is_capture || moved_ptype == Pawn) {
        game.halfmove_clock++;
    }

    if constexpr (MOVING_COLOR == Color::Black) {
        game.fullmoves++;
    }

    game.to_move = !game.to_move;
    hash_update_color_to_move(game.hash);

    return;
}

export __forceinline void make_move(Game& game, Move move)
{
    if (game.to_move == Color::White) {
        make_move_internal<Color::White>(game, move);
    }
    else {
        make_move_internal<Color::Black>(game, move);
    }
}
