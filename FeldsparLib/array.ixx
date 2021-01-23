export module unstd.array;

import unstd.core;

template <typename T>
class DynArrayBase {
protected:
    T* m_ptr = nullptr;
    U64 m_length = 0;
    U64 m_capacity = 0;

    DynArrayBase(T* ptr, U64 length, U64 capacity)
        : m_ptr(ptr), m_length(length), m_capacity(capacity)
    {
    }

    virtual void reserve_nocheck(U64 new_capacity) = 0;

public:
    __forceinline const T* const ptr() const { return m_ptr; }
    __forceinline T* ptr() { return m_ptr; }

    constexpr __forceinline U64 capacity() const { return m_capacity; }
    constexpr __forceinline U64 length() const { return m_length; }

    constexpr __forceinline const T& operator[](U64 idx) const { return m_ptr[idx]; }
    constexpr __forceinline T& operator[](U64 idx) { return m_ptr[idx]; }
    constexpr __forceinline T* begin() { return m_ptr; }
    constexpr __forceinline T* end() { return m_ptr + m_length; }

    virtual void clear() = 0;

    void reserve(U64 new_capacity)
    {
        if (new_capacity > m_capacity) {
            reserve_nocheck(new_capacity);
        }
    }

    __forceinline void append(const T& val)
    {
        if (m_length >= m_capacity) [[unlikely]] {
            reserve_nocheck(2 * m_capacity);
        }

        new (m_ptr + m_length) T(val);
        m_length++;
    }

    // __forceinline void concat(const T* const data, U64 count)
    // {
    //     reserve(m_length + count);
    //     for (U64 idx = 0; idx < count; idx++) {
    //         new (m_ptr + m_length + idx) T(*data[idx]);
    //     }
    //     m_length += count;
    // }

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

export template <typename T, U64 STACK_SIZE = 0>
class DynArray final : public DynArrayBase<T> {
    T m_data[STACK_SIZE];
    bool m_on_stack;

    void reserve_nocheck(U64 new_capacity)
    {
        T* new_ptr = static_cast<T*>(malloc(sizeof(T) * new_capacity));
        for (U64 i = 0; i < this->m_length; i++) {
            new (new_ptr + i) T(this->m_ptr[i]);
        }

        if (m_on_stack) {
            m_on_stack = false;
        }
        else {
            free(this->m_ptr);
        }

        this->m_ptr = new_ptr;
        this->m_capacity = new_capacity;
    }

public:
    DynArray(void) : DynArrayBase<T>(&m_data[0], 0, STACK_SIZE), m_on_stack(true) {}

    DynArray(const T* const chunk, U64 len) : DynArray()
    {
        this->reserve(len);

        for (U64 idx = 0; idx < len; idx++) {
            new (this->m_ptr + idx) T(*chunk[idx]);
        }

        this->m_length += len;
    }

    constexpr __forceinline bool on_stack() const { return m_on_stack; }

    void clear()
    {
        for (U64 i = 0; i < this->m_length; i++) {
            this->m_ptr[i].~T();
        }

        if (!m_on_stack) free(this->m_ptr);

        this->m_length = 0;
        this->m_capacity = STACK_SIZE;
        this->m_ptr = &m_data[0];
        m_on_stack = true;
    }
};

export template <typename T>
class DynArray<T, 0> final : public DynArrayBase<T> {
    void reserve_nocheck(U64 new_capacity)
    {
        // TODO: use fatal_error function to print message and exit if
        // no memory is available
        T* new_ptr = static_cast<T*>(malloc(sizeof(T) * new_capacity));
        for (U64 i = 0; i < this->m_length; i++) {
            new (new_ptr + i) T(this->m_ptr[i]);
        }

        free(this->m_ptr);
        this->m_ptr = new_ptr;
        this->m_capacity = new_capacity;
    }

public:
    DynArray(void) : DynArrayBase<T>(nullptr, 0, 0) {}

    DynArray(const T* const chunk, U64 len) : DynArray()
    {
        this->reserve(len);

        for (U64 idx = 0; idx < len; idx++) {
            new (this->m_ptr + idx) T(*chunk[idx]);
        }

        this->m_length += len;
    }

    void clear()
    {
        for (U64 i = 0; i < this->m_length; i++) {
            this->m_ptr[i].~T();
        }

        this->m_length = 0;
    }
};

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
