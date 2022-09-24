export module unstd.core;

#pragma warning(disable : 5050)
#include "macros.h"

import <cstdint>;

export using U64 = uint64_t;
export using U32 = uint32_t;
export using U16 = uint16_t;
export using U8 = uint8_t;
static_assert(sizeof(U64) == 8);
static_assert(sizeof(U32) == 4);
static_assert(sizeof(U16) == 2);
static_assert(sizeof(U8) == 1);

export using S64 = int64_t;
export using S32 = int32_t;
export using S16 = int16_t;
export using S8 = int8_t;
static_assert(sizeof(S64) == 8);
static_assert(sizeof(S32) == 4);
static_assert(sizeof(S16) == 2);
static_assert(sizeof(S8) == 1);

export using F32 = float;
export using F64 = double;
static_assert(sizeof(F64) == 8);
static_assert(sizeof(F32) == 4);

// For any enum class that defines a LAST member, this allows us to loop over the enum
// easily in a range-based for loop: for (const auto x : EnumRange<T>()).
// Luckily optimizes out to be equivalent to incrementing a single integer (on MSVC).
export template <class T>
struct EnumRange {
    struct Iterator {
        explicit constexpr __ALWAYS_INLINE Iterator(int v) : value(v) {}
        constexpr __ALWAYS_INLINE void operator++() { ++value; }
        constexpr __ALWAYS_INLINE bool operator!=(Iterator rhs) { return value != rhs.value; }
        constexpr __ALWAYS_INLINE T operator*() const { return static_cast<T>(value); }

        int value = 0;
    };

    constexpr __ALWAYS_INLINE Iterator begin() const { return Iterator(0); }
    constexpr __ALWAYS_INLINE Iterator end() const { return Iterator(static_cast<int>(T::LAST) + 1); }
};

// Same idea as std::optional<T>, but for small types where one can easily define a
// 'sentinel' value to represent an empty state. This allows for the same size/alignment
// for both T and Maybe<T> and thus efficient processing in arrays and less size
// taken up in structs.
export template <typename T, T SENTINEL>
class alignas(alignof(T)) Maybe {
    T m_data;

public:
    [[nodiscard]] __ALWAYS_INLINE bool has_value() const { return m_data != SENTINEL; }

    __ALWAYS_INLINE void clear() { m_data = SENTINEL; }

    [[nodiscard]] __ALWAYS_INLINE T& operator*()
    {
        //assert(m_data != SENTINEL);
        return m_data;
    }

    [[nodiscard]] __ALWAYS_INLINE const T& operator*() const
    {
        //assert(m_data != SENTINEL);
        return m_data;
    }

    __ALWAYS_INLINE constexpr Maybe(void) : m_data(SENTINEL) {}
    __ALWAYS_INLINE constexpr Maybe(const T& value) : m_data(value) {}
    __ALWAYS_INLINE constexpr Maybe(const Maybe<T, SENTINEL>& other) : m_data(other.m_data) {}

    __ALWAYS_INLINE Maybe operator=(const Maybe& other)
    {
        m_data = other.m_data;
        return *this;
    }

    __ALWAYS_INLINE Maybe& operator=(Maybe&& other) noexcept
    {
        m_data = other.m_data;
        return *this;
    }

    __ALWAYS_INLINE bool operator==(const Maybe<T, SENTINEL>& other) const
    {
        return this->m_data == other.m_data;
    }
};
