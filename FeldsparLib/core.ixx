export module unstd.core;

import<cassert>;
import<cstdint>;

export using U64 = uint64_t;
export using U32 = uint32_t;
export using U16 = uint16_t;
export using U8 = uint8_t;
export using S64 = int64_t;
export using S32 = int32_t;
export using S16 = int16_t;
export using S8 = int8_t;

export using F32 = float;
export using F64 = double;

static_assert(sizeof(U64) == 8);
static_assert(sizeof(U32) == 4);
static_assert(sizeof(U16) == 2);
static_assert(sizeof(U8) == 1);

export template <typename T>
const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

export template <typename T>
struct WrappedConstPtr {
    const T* const p;
    WrappedConstPtr(const T* const _p) : p(_p) {}
};

// For any enum class that defines a LAST member, this allows us to loop over the enum
// easily in a range-based for loop: for (const auto x : EnumRange<T>()).
// Luckily optimizes out to be equivalent to incrementing a single integer (on MSVC).
export template <class T>
struct EnumRange {
    struct Iterator {
        explicit __forceinline constexpr Iterator(int v) : value(v) {}
        __forceinline constexpr void operator++() { ++value; }
        __forceinline constexpr bool operator!=(Iterator rhs) { return value != rhs.value; }
        __forceinline constexpr T operator*() const { return static_cast<T>(value); }

        int value = 0;
    };

    constexpr __forceinline Iterator begin() const { return Iterator(0); }
    constexpr __forceinline Iterator end() const { return Iterator(static_cast<int>(T::LAST) + 1); }
};

export template <typename T>
class alignas(alignof(T)) [[nodiscard]] Optional {
    U8 m_data[sizeof(T)];
    bool m_filled;

    __forceinline T* cast_data(void) { return reinterpret_cast<T*>(&m_data); }
    __forceinline const T* cast_data(void) const { return reinterpret_cast<const T*>(&m_data); }

public:
    [[nodiscard]] __forceinline bool has_value() const { return m_filled; }

    __forceinline void clear() { m_filled = false; }

    [[nodiscard]] __forceinline T& operator*()
    {
        assert(m_filled);
        return *cast_data();
    }

    [[nodiscard]] __forceinline const T& operator*() const
    {
        assert(m_filled);
        const T* data = cast_data();
        return *data;
    }

    __forceinline constexpr Optional(void) : m_filled(false) {}
    __forceinline constexpr Optional(const T& value) { new (cast_data()) T(value); }

    __forceinline constexpr Optional(const Optional<T>& other)
    {
        if (m_filled = other.m_filled) {
            new (cast_data()) T(*other);
        }
    }

    __forceinline Optional& operator=(const Optional& other)
    {
        if (m_filled = other.m_filled) {
            memcpy(&m_data, &other.m_data, sizeof(T));
        }
        return *this;
    }

    __forceinline bool operator==(const Optional<T>& other) const
    {
        if (this->m_filled != other.m_filled) {
            return false;
        }

        if (!this->m_filled) return true;

        return this->m_data == other.m_data;
    }
};

// Same behavior as Optional<T>, but for small types where one can easily define a
// 'sentinel' value to represent an empty state. This allows for the same size/alignment
// for both T and Maybe<T> and thus efficient processing in arrays.
export template <typename T, T SENTINEL>
class alignas(alignof(T)) [[nodiscard]] Maybe {
    T m_data;

public:
    [[nodiscard]] __forceinline bool has_value() const { return m_data != SENTINEL; }

    __forceinline void clear() { m_data = SENTINEL; }

    [[nodiscard]] __forceinline T& operator*()
    {
        assert(m_data != SENTINEL);
        return m_data;
    }

    __forceinline constexpr Maybe(void) : m_data(SENTINEL) {}
    __forceinline constexpr Maybe(const T& value) : m_data(value) {}
    __forceinline constexpr Maybe(const Maybe<T, SENTINEL>& other) : m_data(other.m_data) {}

    __forceinline Maybe operator=(const Maybe& other)
    {
        m_data = other.m_data;
        return *this;
    }

    __forceinline Maybe& operator=(Maybe&& other) noexcept
    {
        m_data = other.m_data;
        return *this;
    }

    __forceinline bool operator==(const Maybe<T, SENTINEL>& other) const
    {
        return this->m_data == other.m_data;
    }
};
