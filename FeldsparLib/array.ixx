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

template <typename T>
class DynArrayBase {
protected:
    T* m_ptr = nullptr;
    U64 m_length = 0;
    U64 m_capacity = 0;

    virtual void grow() = 0;

public:
    virtual void clear() = 0;

    virtual const T* const ptr() const { return m_ptr; }
    virtual T* ptr() { return m_ptr; }

    constexpr __forceinline U64 capacity() const { return m_capacity; }
    constexpr __forceinline U64 length() const { return m_length; }

    constexpr __forceinline const T& operator[](U64 idx) const { return m_ptr[idx]; }
    constexpr __forceinline T& operator[](U64 idx) { return m_ptr[idx]; }
    constexpr __forceinline T* begin() { return m_ptr; }
    constexpr __forceinline T* end() { return m_ptr + m_length; }

    __forceinline void append(const T& val)
    {
        if (m_length >= m_capacity) [[unlikely]] {
            grow();
        }

        new (ptr() + m_length) T(val);
        m_length++;
    }

    template <U64 M>
    constexpr bool operator==(const T (&list)[M]) const
    {
        if (M != m_length) return false;

        for (U64 i = 0; i < m_length; i++) {
            if (list[i] != ptr()[i]) return false;
        }

        return true;
    }
};

export template <typename T, U64 STACK_SIZE = 0>
class DynArray : public DynArrayBase<T> {
    T m_data[STACK_SIZE];
    bool m_on_stack;

    void grow()
    {
        const U64 new_capacity = this->m_capacity * 2;

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
    DynArray(void)
    {
        this->m_length = 0;
        this->m_capacity = STACK_SIZE;
        this->m_on_stack = true;
        this->m_ptr = &m_data[0];
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
    void grow()
    {
        const U64 new_capacity = this->m_capacity > 0 ? this->m_capacity * 2 : 2;

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
    DynArray(void)
    {
        this->m_length = 0;
        this->m_capacity = 0;
        this->m_ptr = nullptr;
    }

    void clear()
    {
        for (U64 i = 0; i < this->m_length; i++) {
            this->m_ptr[i].~T();
        }

        this->m_length = 0;
    }
};
