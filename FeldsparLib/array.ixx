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

    inline U64 capacity() const { return N; }

    const T& operator[](U64 idx) const { return m_data[idx]; }
    T& operator[](U64 idx) { return m_data[idx]; }

    T* begin() { return m_data; }
    T* end() { return m_data + N; }
};

export template <typename T, U64 STACK_SIZE>
class DynArray {
    alignas(alignof(T)) U8 m_data[sizeof(T) * STACK_SIZE];
    T* m_ptr;

    U64 m_length;
    U64 m_capacity;
    bool m_on_stack;

    void grow_if_needed()
    {
        if (m_length < m_capacity) return;

        const U64 new_capacity = m_capacity * 2;

        if (m_on_stack) {
            m_ptr = static_cast<T*>(malloc(sizeof(T) * new_capacity));
            m_on_stack = false;
            m_capacity = new_capacity;
            return;
        }

        T* new_ptr = static_cast<T*>(malloc(sizeof(T) * new_capacity));
        for (U64 i = 0; i < m_length; i++) {
            new (new_ptr + i) T(std::move(m_ptr[i]));
        }

        free(m_ptr);
        m_ptr = new_ptr;
        m_capacity = new_capacity;
    }

public:
    DynArray(void)
        : m_length(0), m_capacity(STACK_SIZE), m_on_stack(true),
          m_ptr(reinterpret_cast<T*>(&m_data))
    {
    }

    template <typename... Args>
    void append(Args&&... args)
    {
        grow_if_needed();

        new (m_ptr + m_length) T(std::forward<Args>(args)...);
        m_length++;
    }

    inline U64 capacity() const { return m_capacity; }

    inline U64 length() const { return m_length; }

    inline const T& operator[](U64 idx) const { return m_ptr[idx]; }
    inline T& operator[](U64 idx) { return m_ptr[idx]; }

    inline T* begin() { return m_ptr; }
    inline T* end() { return m_ptr + m_length; }

    inline bool on_stack() const { return m_on_stack; }

    void clear()
    {
        for (U64 i = 0; i < m_length; i++) {
            m_ptr[i].~T();
        }

        if (!m_on_stack) free(m_ptr);

        m_length = 0;
        m_capacity = STACK_SIZE;
        m_ptr = reinterpret_cast<T*>(&m_data);
        m_on_stack = true;
    }
};