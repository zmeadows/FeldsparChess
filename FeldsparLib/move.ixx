export module move;

import prelude;
import bitboard;
import board;
import game;
import zobrist;

import unstd.string;

import<cassert>;

export class MoveBuffer {
    Move moves[256];
    U64 m_count = 0;

public:
    constexpr __forceinline void append(Move m) { moves[m_count++] = m; }
    constexpr __forceinline U64 length() const { return m_count; }
    constexpr __forceinline void clear() { m_count = 0; }

    constexpr __forceinline const Move* const begin() const { return &moves[0]; }
    constexpr __forceinline const Move* const end() const { return &moves[0] + m_count; }
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
export __forceinline constexpr Move create_quiet_move(Square from, Square to, U32 flag,
                                                      PieceType moved_ptype)
{
    const U32 to_bits = static_cast<U32>(to);
    const U32 from_bits = (static_cast<U32>(from) << 6);
    const U32 flag_bits = flag << 12;
    const U32 ptype_bits = static_cast<U32>(moved_ptype) << 16;

    return ptype_bits | flag_bits | from_bits | to_bits;
}

// TODO: add assert statements to validate these function inputs
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

// TODO: add tests for all of these
export __forceinline constexpr U32 move_flag(Move move) { return (move >> 12) & 0xF; }

export __forceinline constexpr bool move_is_capture(Move move)
{
    return (move_flag(move) & 0b0100) != 0;
}

export __forceinline constexpr bool move_is_pawn_promotion(Move move)
{
    return (move_flag(move) & 0b1000) != 0;
}

export __forceinline constexpr Square move_from_square(Move move) { return (move >> 6) & 0x3F; }

export __forceinline constexpr Square move_to_square(Move move) { return move & 0x3F; }

export __forceinline constexpr PieceType moved_piece_type(Move move)
{
    return static_cast<PieceType>((move >> 16) & 0x7);
}

// NOTE: this will be equal to PieceType::Pawn if the move is not a capture. How bad is that...?
export __forceinline constexpr PieceType captured_piece_type(Move move)
{
    return static_cast<PieceType>((move >> 19) & 0x7);
}

template <Color MOVING_COLOR, bool UPDATE_HASH>
void make_move_internal(Game& game, Move move)
{
    using enum PieceType;

    // TODO: only define these constants at the scope required
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

    const auto remove_castling_rights = [&](CastlingRights remove_mask) {
        if constexpr (UPDATE_HASH) hash_update_castling_rights(game.hash, game.castling_rights);
        game.castling_rights &= remove_mask;
        if constexpr (UPDATE_HASH) hash_update_castling_rights(game.hash, game.castling_rights);
    };

    // TODO: should we not be updating hash for the to_sq on pawn promotion?
    if constexpr (UPDATE_HASH) {
        hash_update_piece_square(game.hash, MOVING_COLOR, moved_ptype, from_sq);
        hash_update_piece_square(game.hash, MOVING_COLOR, moved_ptype, to_sq);
    }
    get_pieces_mut(game.board, moved_ptype, MOVING_COLOR) ^= from_to_bit;
    get_occupied_mut(game.board, MOVING_COLOR) ^= from_to_bit;

    // En Passant square will always change or disappear each move, if it exists
    if constexpr (UPDATE_HASH) {
        if (game.ep_square.has_value()) {
            hash_update_ep_square(game.hash, *game.ep_square);
        }
    }

    if (is_capture) [[unlikely]] {
        Bitboard captured_bit;
        Square captured_sq;

        if (flag != EP_CAPTURE_FLAG) [[likely]] {
            captured_bit = to_bit;
            captured_sq = to_sq;
        }
        else {
            assert(game.ep_square.has_value() &&
                   "En Passant capture move attempted when Game has no ep_square.");

            if constexpr (MOVING_COLOR == Color::White) {
                captured_sq = (*game.ep_square) - 8;
            }
            else { // MOVING_COLOR == Color::Black
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
    }

    switch (moved_ptype) {
        case Pawn: {
            if (flag == DOUBLE_PAWN_PUSH_FLAG) {
                if constexpr (MOVING_COLOR == Color::White) {
                    game.ep_square = to_sq - 8;
                }
                else { // MOVING_COLOR == Color::Black
                    game.ep_square = to_sq + 8;
                }

                if constexpr (UPDATE_HASH) {
                    hash_update_ep_square(game.hash, *game.ep_square);
                }
            }
            else if (move_is_pawn_promotion(move)) [[unlikely]] {
                if (flag == KNIGHT_PROMO_FLAG || flag == KNIGHT_PROMO_CAPTURE_FLAG) {
                }
                else if (flag == BISHOP_PROMO_FLAG || flag == BISHOP_PROMO_CAPTURE_FLAG) {
                }
                else if (flag == ROOK_PROMO_FLAG || flag == ROOK_PROMO_CAPTURE_FLAG) {
                }
                else if (flag == QUEEN_PROMO_FLAG || flag == QUEEN_PROMO_CAPTURE_FLAG) {
                }
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
        game.halfmove_clock = 0;
    }
    else {
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
__forceinline void make_move(Game& game, Move move)
{
    if (game.to_move == Color::White) {
        make_move_internal<Color::White, UPDATE_HASH>(game, move);
    }
    else {
        make_move_internal<Color::Black, UPDATE_HASH>(game, move);
    }
}

export String<4> move_to_algebraic(Move m)
{
    const Square from = move_from_square(m);
    const Square to = move_to_square(m);

    StringRef from_alg = square_to_algebraic(from);
    StringRef to_alg = square_to_algebraic(to);

    // TODO: create String::concat method
    String<4> alg_str;
    alg_str.append(from_alg[0]);
    alg_str.append(from_alg[1]);
    alg_str.append(to_alg[0]);
    alg_str.append(to_alg[1]);

    return alg_str;
}
