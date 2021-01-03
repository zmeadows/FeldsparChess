export module unstd.core;

import<cassert>;
import<cstdint>;
import<utility>;

export using U64 = uint64_t;
export using U32 = uint32_t;
export using U16 = uint16_t;
export using U8 = uint8_t;
export using S64 = int64_t;
export using S32 = int32_t;
export using S16 = int16_t;
export using S8 = int8_t;

// For any enum class that defines a LAST member, this allows us to loop over the enum
// easily in a range-based for loop: for (const auto x : EnumRange<T>()).
// Luckily optimizes out to be equivalent to incrementing a single integer (on MSVC).
export template <class T>
struct EnumRange {
    struct Iterator {
        explicit inline constexpr Iterator(int v) : value(v) {}
        inline void operator++() { ++value; }
        inline bool operator!=(Iterator rhs) { return value != rhs.value; }
        inline T operator*() const { return static_cast<T>(value); }

        int value = 0;
    };

    constexpr inline Iterator begin() const { return Iterator(0); }
    constexpr inline Iterator end() const { return Iterator(static_cast<int>(T::LAST) + 1); }
};

export template <typename T>
class alignas(alignof(T)) [[nodiscard]] Optional {
    U8 m_data[sizeof(T)];
    bool m_filled;

    inline T* cast_data(void) { return reinterpret_cast<T*>(&m_data); }

public:
    [[nodiscard]] inline bool has_value() const { return m_filled; }

    inline void clear() { m_filled = false; }

    [[nodiscard]] inline T& operator*()
    {
        assert(m_filled);
        return *cast_data();
    }

    inline constexpr Optional(void) : m_filled(false) {}
    inline constexpr Optional(const T& value) { new (cast_data()) T(value); }
    inline constexpr Optional(T&& value) { new (cast_data()) T(std::move(value)); }

    inline constexpr Optional(const Optional<T>& other)
    {
        if (m_filled = other.m_filled) {
            new (cast_data()) T(*other);
        }
    }

    inline constexpr Optional(Optional<T>&& other)
    {
        if (m_filled = other.m_filled) {
            new (cast_data()) T(std::move(*other));
        }
    }

    inline Optional& operator=(const Optional& other)
    {
        if (m_filled = other.m_filled) {
            memcpy(&m_data, &other.m_data, sizeof(T));
        }
        return *this;
    }

    inline Optional& operator=(Optional&& other)
    {
        if (m_filled = other.m_filled) {
            memcpy(&m_data, &other.m_data, sizeof(T));
        }
        return *this;
    }
};

// Same behavior as Optional<T>, but for small types where one can easily define a
// 'sentinel' value to represent an empty state. This allows for the same size/alignment
// for both T and Maybe<T> and thus efficient processing in arrays.
export template <typename T, T SENTINEL>
class alignas(alignof(T)) [[nodiscard]] Maybe {
    T m_data;

public:
    [[nodiscard]] inline bool has_value() { return m_data != SENTINEL; }

    inline void clear() { m_data = SENTINEL; }

    [[nodiscard]] inline T& operator*()
    {
        assert(m_data != SENTINEL);
        return m_data;
    }

    inline constexpr Maybe(void) : m_data(SENTINEL) {}

    inline explicit constexpr Maybe(const T& value) : m_data(value) {}
    inline explicit constexpr Maybe(T&& value) : m_data(std::move(value)) {}

    inline constexpr Maybe(const Maybe<T, SENTINEL>& other) : m_data(other.m_data) {}
    inline constexpr Maybe(Maybe<T, SENTINEL>&& other) : m_data(std::move(other.m_data)) {}

    inline Maybe operator=(const Maybe& other)
    {
        m_data = other.m_data;
        return *this;
    }

    inline Maybe& operator=(Maybe&& other)
    {
        m_data = std::move(other.m_data);
        return *this;
    }
};
