#include <cassert>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define USING_WINDOWS true
#else
#define USING_WINDOWS false
#endif

#define __SHORT_FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define WARN(msg)                                                                                  \
    do {                                                                                           \
        if constexpr (USING_WINDOWS) {                                                             \
            to_stdout("[\x1B[31mWARNING\033[0m] {}:{} :: {}\n", __SHORT_FILENAME__, __LINE__, msg);   \
        } else {                                                                                   \
            to_stdout("[WARNING] {}:{} :: {}\n", __SHORT_FILENAME__, __LINE__, msg);                  \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_BB(flag, x)                                                                          \
    do {                                                                                           \
        if constexpr (flag) {                                                               \
            to_stdout("{}:\n", #x);                                                                   \
            print_bitboard(x);                                                                     \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_BB2(x, descr)                                                                  \
    do {                                                                                           \
        if constexpr (DEBUG_PRINT) {                                                               \
            to_stdout("{} ({}):\n", #x, descr);                                                       \
            print_bitboard(x);                                                                     \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_QBB(x)                                                                         \
    do {                                                                                           \
        if constexpr (DEBUG_PRINT) {                                                               \
            to_stdout("{}:\n", #x);                                                                   \
            print_quad_bitboard(x);                                                                \
        }                                                                                          \
    } while (0)

#define DEBUG_PRINT_QBB2(x, descr)                                                                 \
    do {                                                                                           \
        if constexpr (DEBUG_PRINT) {                                                               \
            to_stdout("{} ({}):\n", #x, descr);                                                       \
            print_quad_bitboard(x);                                                                \
        }                                                                                          \
    } while (0)
