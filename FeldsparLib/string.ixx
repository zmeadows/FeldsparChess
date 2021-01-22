export module unstd.string;

import unstd.core;
import unstd.array;

import<cstring>;

export template <U64 N = 0>
class String {
    DynArray<char, N> m_data = {};

public:
    explicit String(const char* str) {}
};

export class StringRef {
    const char* const m_str = nullptr;
    const U64 m_length = 0;

public:
    template <U64 N>
    explicit StringRef(const String<N>& s) : StringRef(s.ptr())
    {
    }
    explicit StringRef(const char* const) {}
};

export template <U64 N, U64 EXPECTED_WORD_COUNT = 0>
DynArray<StringRef, EXPECTED_WORD_COUNT> split(const String<N>& str, const char sep = ' ')
{
    DynArray<StringRef, EXPECTED_WORD_COUNT> words;

    return words;
}
