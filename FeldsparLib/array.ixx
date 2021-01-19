export module unstd.array;

import unstd.core;
import<utility>;

// TODO: big five for both Array and DynArray
export template <typename T, U64 N>
class Array {
    T m_data[N];

public:
    constexpr Array(void) {}
    constexpr Array(const T& value) : m_data({value}) {}

    template <U64 M>
    constexpr Array(const T (&list)[M])
    {
        static_assert(M == N, "Attempted to initialize a fixed length Array with incorrect size.");
        for (U64 i = 0; i < N; i++) {
            m_data[i] = list[i];
        }
    }

    inline constexpr U64 capacity() const { return N; }

    constexpr inline const T& operator[](U64 idx) const { return m_data[idx]; }
    constexpr inline T& operator[](U64 idx) { return m_data[idx]; }
    constexpr inline T* begin() { return m_data; }
    constexpr inline T* end() { return m_data + N; }
};

export template <typename T, U64 STACK_SIZE>
class DynArray {
    T m_data[STACK_SIZE];
    T* m_ptr;

    U64 m_length;
    U64 m_capacity;
    bool m_on_stack;

    void grow()
    {
        const U64 new_capacity = m_capacity * 2;

        T* new_ptr = static_cast<T*>(malloc(sizeof(T) * new_capacity));
        for (U64 i = 0; i < m_length; i++) {
            new (new_ptr + i) T(std::move(m_ptr[i]));
        }

        if (m_on_stack) {
            m_on_stack = false;
        }
        else {
            free(m_ptr);
        }

        m_ptr = new_ptr;
        m_capacity = new_capacity;
    }

public:
    DynArray(void) : m_length(0), m_capacity(STACK_SIZE), m_on_stack(true), m_ptr(&m_data[0]) {}

    template <typename... Args>
    inline void append_move(Args&&... args)
    {
        if (m_length >= m_capacity) [[unlikely]] {
            grow();
        }

        new (m_ptr + m_length) T(std::forward<Args>(args)...);
        m_length++;
    }

    inline void append(const T& val)
    {
        if (m_length >= m_capacity) [[unlikely]] {
            grow();
        }

        new (m_ptr + m_length) T(val);
        m_length++;
    }

    constexpr inline U64 capacity() const { return m_capacity; }

    constexpr inline U64 length() const { return m_length; }

    constexpr inline const T& operator[](U64 idx) const { return m_ptr[idx]; }
    constexpr inline T& operator[](U64 idx) { return m_ptr[idx]; }
    constexpr inline T* begin() { return m_ptr; }
    constexpr inline T* end() { return m_ptr + m_length; }

    constexpr inline bool on_stack() const { return m_on_stack; }

    void clear()
    {
        for (U64 i = 0; i < m_length; i++) {
            m_ptr[i].~T();
        }

        if (!m_on_stack) free(m_ptr);

        m_length = 0;
        m_capacity = STACK_SIZE;
        m_ptr = &m_data[0];
        m_on_stack = true;
    }

    template <U64 M>
    constexpr bool operator==(const T (&list)[M]) const
    {
        if (M != m_length) return false;

        for (U64 i = 0; i < m_length; i++) {
            if (list[i] != m_data[i]) return false;
        }

        return true;
    }
};