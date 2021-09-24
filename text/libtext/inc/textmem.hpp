#ifndef LIBTEXT_TEXT_MEMORY_H
#define LIBTEXT_TEXT_MEMORY_H

#include "stringpool.hpp"
#include "stringblock.hpp"

namespace text {
    struct text_mem {
        static inline constexpr size_t fragsize = 1024;
        static inline constexpr size_t fragcount = 512;
        static inline auto defualt_str_pool = basic_string_pool<fragsize, char>(fragcount);
        static inline auto defualt_wstr_pool = basic_string_pool<fragsize, char16_t>(fragcount);

        static inline const auto default_str_alloc = basic_string_pool_allocate_fn<char>([]() { return text_mem::defualt_str_pool.allocate(); });
        static inline const auto default_str_dealloc = basic_string_pool_deallocate_fn<char>([](auto&& frag) { return text_mem::defualt_str_pool.deallocate(std::forward<decltype(frag)>(frag)); });
        static inline const auto default_str_fragsize = basic_string_pool_fragsize_fn([]() { return text_mem::defualt_str_pool.fragment_size; });
        static inline const auto default_wstr_alloc = basic_string_pool_allocate_fn<char16_t>([]() { return text_mem::defualt_wstr_pool.allocate(); });
        static inline const auto default_wstr_dealloc = basic_string_pool_deallocate_fn<char16_t>([](auto&& frag) { return text_mem::defualt_wstr_pool.deallocate(std::forward<decltype(frag)>(frag)); });
        static inline const auto default_wstr_fragsize = basic_string_pool_fragsize_fn([]() { return text_mem::defualt_wstr_pool.fragment_size; });
    };

    template<class _CharT>
    basic_string_block<_CharT> make_string_block();
    template<>
    basic_string_block<char> make_string_block<char>() {
        return basic_string_block(
            text_mem::default_str_alloc,
            text_mem::default_str_dealloc,
            text_mem::default_str_fragsize);
    }
    template<>
    basic_string_block<char16_t> make_string_block<char16_t>() {
        return basic_string_block(
            text_mem::default_wstr_alloc,
            text_mem::default_wstr_dealloc,
            text_mem::default_wstr_fragsize);
    }

    template<class _CharT>
    basic_string_block<_CharT> make_string_block(const std::basic_string<_CharT>& initstr);
    template<>
    basic_string_block<char> make_string_block<char>(const std::basic_string<char>& initstr) {
        return basic_string_block(
            text_mem::default_str_alloc,
            text_mem::default_str_dealloc,
            text_mem::default_str_fragsize,
            initstr);
    }
    template<>
    basic_string_block<char16_t> make_string_block<char16_t>(const std::basic_string<char16_t>& initstr) {
        return basic_string_block(
            text_mem::default_wstr_alloc,
            text_mem::default_wstr_dealloc,
            text_mem::default_wstr_fragsize,
            initstr);
    }
}

#endif // !LIBTEXT_TEXT_MEMORY_H
