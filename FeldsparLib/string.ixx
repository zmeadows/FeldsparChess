export module unstd.string;

import unstd.core;
import unstd.array;

import<cstdlib>;
import<cstring>;

constexpr U64 MAX_STRING_SIZE = (U64)1e8;

export template <U64 N = 0>
class String : public DynArray<char, N> {

public:
    String(void) {}
    String(WrappedConstPtr<char> cstr)
        : DynArray<char, N>(cstr.p, strnlen_s(cstr.p, MAX_STRING_SIZE))
    {
    }

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
    StringRef() : m_str(nullptr), m_length(0) {}

    explicit StringRef(const char* const str, U64 length) : m_str(str), m_length(length) {}

    template <U64 N>
    explicit StringRef(const String<N>& s) : StringRef(s.ptr(), s.length())
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
};

export template <U64 WORDC = 0, U64 N>
DynArray<StringRef, WORDC> split(const String<N>& str, const char sep = ' ')
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

        words.append(StringRef(str.ptr() + start_idx, end_idx - start_idx));
    }

    return words;
}
