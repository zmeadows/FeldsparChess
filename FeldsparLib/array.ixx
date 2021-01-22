export module unstd.array;

import unstd.core;

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

    __forceinline constexpr U64 capacity() const { return N; }

    constexpr __forceinline const T& operator[](U64 idx) const { return m_data[idx]; }
    constexpr __forceinline T& operator[](U64 idx) { return m_data[idx]; }
    constexpr __forceinline T* begin() { return m_data; }
    constexpr __forceinline T* end() { return m_data + N; }
};

export template <typename T, U64 STACK_SIZE = 0>
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
            new (new_ptr + i) T(m_ptr[i]);
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

    __forceinline const T* const ptr() { return m_ptr; }

    __forceinline void append(const T& val)
    {
        if (m_length >= m_capacity) [[unlikely]] {
            grow();
        }

        new (m_ptr + m_length) T(val);
        m_length++;
    }

    constexpr __forceinline U64 capacity() const { return m_capacity; }

    constexpr __forceinline U64 length() const { return m_length; }

    constexpr __forceinline const T& operator[](U64 idx) const { return m_ptr[idx]; }
    constexpr __forceinline T& operator[](U64 idx) { return m_ptr[idx]; }
    constexpr __forceinline T* begin() { return m_ptr; }
    constexpr __forceinline T* end() { return m_ptr + m_length; }

    constexpr __forceinline bool on_stack() const { return m_on_stack; }

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

export template <typename T>
class DynArray<T, 0> {
    T* m_ptr;
    U64 m_length;
    U64 m_capacity;

    void grow()
    {
        const U64 new_capacity = m_capacity > 0 ? m_capacity * 2 : 2;

        // TODO: use fatal_error function to print message and exit if
        // no memory is available
        T* new_ptr = static_cast<T*>(malloc(sizeof(T) * new_capacity));
        for (U64 i = 0; i < m_length; i++) {
            new (new_ptr + i) T(m_ptr[i]);
        }

        free(m_ptr);
        m_ptr = new_ptr;
        m_capacity = new_capacity;
    }

public:
    DynArray(void) : m_length(0), m_capacity(0), m_ptr(nullptr) {}

    __forceinline void append(const T& val)
    {
        if (m_length >= m_capacity) [[unlikely]] {
            grow();
        }

        new (m_ptr + m_length) T(val);
        m_length++;
    }

    constexpr __forceinline U64 capacity() const { return m_capacity; }

    constexpr __forceinline U64 length() const { return m_length; }

    constexpr __forceinline const T& operator[](U64 idx) const { return m_ptr[idx]; }
    constexpr __forceinline T& operator[](U64 idx) { return m_ptr[idx]; }
    constexpr __forceinline T* begin() { return m_ptr; }
    constexpr __forceinline T* end() { return m_ptr + m_length; }

    void clear()
    {
        for (U64 i = 0; i < m_length; i++) {
            m_ptr[i].~T();
        }

        m_length = 0;
    }

    template <U64 M>
    constexpr bool operator==(const T (&list)[M]) const
    {
        if (M != m_length) return false;

        for (U64 i = 0; i < m_length; i++) {
            if (list[i] != m_ptr[i]) return false;
        }

        return true;
    }
};
