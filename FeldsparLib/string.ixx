export module unstd.string;

import unstd.core;
import unstd.array;

import<cstring>;

// TODO: has_prefix method

export template <U64 N = 0>
class String : public DynArray<char, N> {

public:
    String(void) {}
    String(WrappedConstPtr<char> cstr) : DynArray<char, N>(cstr.p, strlen(cstr.p)) {}

    template <U64 M>
    explicit String(const char (&list)[M]) : DynArray<char, N>(list, M - 1)
    {
    }

    template <U64 M>
    constexpr inline bool operator==(const char (&list)[M]) const
    {
        if (M != this->m_length + 1) return false;

        for (U64 i = 0; i < this->m_length; i++) {
            if (list[i] != this->m_ptr[i]) return false;
        }

        return true;
    }
};

export class StringRef {
    const char* const m_str;
    const U64 m_length;

public:
    constexpr StringRef() : m_str(nullptr), m_length(0) {}

    StringRef(const char* const str, U64 length) : m_str(str), m_length(length) {}

    template <U64 M>
    constexpr StringRef(const char (&list)[M]) : m_str(list), m_length(M - 1)
    {
    }

    template <U64 N>
    StringRef(const String<N>& s) : StringRef(s.ptr(), s.length())
    {
    }

    template <U64 M>
    constexpr inline bool operator==(const char (&list)[M]) const
    {
        if (M != this->m_length + 1) return false;

        for (U64 i = 0; i < this->m_length; i++) {
            if (list[i] != this->m_str[i]) return false;
        }

        return true;
    }

    constexpr __forceinline U64 length() const { return m_length; }

    StringRef substr(U64 start_idx, U64 length) const
    {
        return StringRef(m_str + start_idx, length);
    }

    constexpr __forceinline const char operator[](U64 idx) { return m_str[idx]; }
    constexpr __forceinline const char* const begin() const { return m_str; }
    constexpr __forceinline const char* const end() const { return m_str + m_length; }

    constexpr __forceinline const char* const cstr() const { return m_str; }
};

export template <U64 WORDC = 0>
DynArray<StringRef, WORDC> split(StringRef str, const char sep = ' ')
{
    DynArray<StringRef, WORDC> words;

    U64 idx = 0;

    while (true) {
        while (idx < str.length() && str[idx] == sep) {
            idx++;
        }

        if (idx == str.length()) {
            break;
        }

        const U64 start_idx = idx;

        while (idx < str.length() && str[idx] != sep) {
            idx++;
        }

        const U64 end_idx = idx;

        words.append(str.substr(start_idx, end_idx - start_idx));
    }

    return words;
}
