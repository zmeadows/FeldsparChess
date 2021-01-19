export module unstd.string;

import unstd.core;
import unstd.array;

import<cstring>;

export class String {
    DynArray<char, 16> m_data = {};

public:
    explicit String(const char* str) {}
};

export class StringRef {
    const char* const m_str = nullptr;
    const U64 m_length = 0;

public:
    explicit StringRef(const String&) {}
    explicit StringRef(const char*) {}
};
