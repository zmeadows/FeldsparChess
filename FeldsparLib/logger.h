#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define USING_WINDOWS true
#endif

#define __SHORT_FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define WARN(msg)                                                                                  \
    do {                                                                                           \
        if constexpr (USING_WINDOWS) {                                                             \
            printf("[\x1B[31mWARNING\033[0m] %s:%d :: %s\n", __SHORT_FILENAME__, __LINE__, msg);   \
        }                                                                                          \
        else {                                                                                     \
            printf("[WARNING] %s:%d :: %s\n", __SHORT_FILENAME__, __LINE__, msg);                  \
        }                                                                                          \
    } while (0)
