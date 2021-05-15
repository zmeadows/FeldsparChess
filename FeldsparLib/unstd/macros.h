#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define USING_WINDOWS true
#endif

#define __SHORT_FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define WARN(msg)                                                                                  \
    do {                                                                                           \
        if constexpr (USING_WINDOWS) {                                                             \
            printf("[\x1B[31mWARNING\033[0m] %s:%d :: %s\n", __SHORT_FILENAME__, __LINE__, msg);   \
        } else {                                                                                   \
            printf("[WARNING] %s:%d :: %s\n", __SHORT_FILENAME__, __LINE__, msg);                  \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_BB(x)                                                                          \
    do {                                                                                           \
        if constexpr (DEBUG_PRINT) {                                                               \
            printf("%s:\n", #x);                                                                   \
            print_bitboard(x);                                                                     \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_BB2(x, descr)                                                                  \
    do {                                                                                           \
        if constexpr (DEBUG_PRINT) {                                                               \
            printf("%s (%s):\n", #x, descr);                                                       \
            print_bitboard(x);                                                                     \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_QBB(x)                                                                         \
    do {                                                                                           \
        if constexpr (DEBUG_PRINT) {                                                               \
            printf("%s:\n", #x);                                                                   \
            print_quad_bitboard(x);                                                                \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_QBB2(x, descr)                                                                 \
    do {                                                                                           \
        if constexpr (DEBUG_PRINT) {                                                               \
            printf("%s (%s):\n", #x, descr);                                                       \
            print_quad_bitboard(x);                                                                \
        }                                                                                          \
    } while (0)

// Due to an unspecified MSVC error I cannot define these as const Bitboard type. Strange.
#define BITBOARD_EMPTY 0x0ULL
#define BITBOARD_FULL 0xFFFFFFFFFFFFFFFFULL
