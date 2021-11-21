#pragma once

#include "textmem.hpp"

template<class _CharT, size_t _FragSize, size_t _FragCount>
inline text::basic_string_block<_CharT> make_test_string_block() {
    static text::basic_string_pool<_FragSize, _CharT> pool(_FragCount);
    return text::basic_string_block<_CharT>(
        []() { return pool.allocate(); },
        [](auto&& frag) { return pool.deallocate(std::forward<decltype(frag)>(frag)); },
        []() { return _FragSize; });
}

