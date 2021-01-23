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
    explicit String(const char* str) : DynArray<char, N>(str, strnlen_s(str, MAX_STRING_SIZE)) {}
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
