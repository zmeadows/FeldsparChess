export module move;

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
