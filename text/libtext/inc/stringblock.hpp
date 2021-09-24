#ifndef LIBTEXT_STRING_BLOCK_H
#define LIBTEXT_STRING_BLOCK_H

#include <cassert>
#include <string>
#include "stringpool.hpp"

namespace text {
    namespace details {
        inline constexpr size_t at_nth_frag(size_t position, size_t frag_size) { return position / frag_size; }
        inline constexpr size_t at_pos_in_frag(size_t position, size_t frag_size) { return position % frag_size; }
        inline constexpr size_t available_length_in_frag(size_t position, size_t frag_size) { return frag_size - at_pos_in_frag(position, frag_size); }
    }

    template<class _CharT>
    class basic_string_block {
    public:
        explicit basic_string_block(
            const basic_string_pool_allocate_fn<_CharT>& allocate,
            const basic_string_pool_deallocate_fn<_CharT>& deallocate,
            const basic_string_pool_fragsize_fn& fragsize);
        explicit basic_string_block(
            const basic_string_pool_allocate_fn<_CharT>& allocate,
            const basic_string_pool_deallocate_fn<_CharT>& deallocate,
            const basic_string_pool_fragsize_fn& fragsize,
            const std::basic_string<_CharT>& init_string);
        ~basic_string_block();
        void append(const std::basic_string<_CharT>& src_str);
    private:
        static inline void _copy_into_frag(
            const basic_string_pool_fragment<_CharT>& frag,
            size_t offset,
            const _CharT* src,
            size_t length);
    private:
        size_t _length;
        std::vector<basic_string_pool_fragment<_CharT>> _frags;
        basic_string_pool_allocate_fn<_CharT> _allocate;
        basic_string_pool_deallocate_fn<_CharT> _deallocate;
        basic_string_pool_fragsize_fn _fragsize;
    };

    using string_block = basic_string_block<char>;
    using wstring_block = basic_string_block<char16_t>;

    // implementations ----------------------------------------------
    /**
     * @brief constructor
     */
    template<class _CharT>
    inline basic_string_block<_CharT>::basic_string_block(
        const basic_string_pool_allocate_fn<_CharT>& allocate,
        const basic_string_pool_deallocate_fn<_CharT>& deallocate,
        const basic_string_pool_fragsize_fn& fragsize)
        : _length(0)
        , _allocate(allocate)
        , _deallocate(deallocate)
        , _fragsize(fragsize)
    {}

    /**
     * @brief constructor
     */
    template<class _CharT>
    inline basic_string_block<_CharT>::basic_string_block(
        const basic_string_pool_allocate_fn<_CharT>& allocate,
        const basic_string_pool_deallocate_fn<_CharT>& deallocate,
        const basic_string_pool_fragsize_fn& fragsize,
        const std::basic_string<_CharT>& init_string)
        : _length(0)
        , _allocate(allocate)
        , _deallocate(deallocate)
        , _fragsize(fragsize) {
        append(init_string);
    }

    template<class _CharT>
    inline basic_string_block<_CharT>::~basic_string_block(){
        for (auto& frag : _frags) {
            _deallocate(std::move(frag));
        }
    }

    template<class _CharT>
    inline void basic_string_block<_CharT>::append(const std::basic_string<_CharT>& src_str) {
        auto len_to_process = src_str.size();
        auto position = _length;
        _length += src_str.length();
        const _CharT* src = src_str.data();
        while (position < _length) {
            auto frag_idx = details::at_nth_frag(position, _fragsize());
            assert(frag_idx == _frags.size() - 1 || frag_idx == _frags.size());
            if (frag_idx == _frags.size()) {
                _frags.emplace_back(_allocate());
            }
            const auto len_available_in_frag = details::available_length_in_frag(position, _fragsize());
            const auto pos_in_frag = details::at_pos_in_frag(position, _fragsize());
            const auto len_to_fill = std::min(_length - position, _fragsize());
            _copy_into_frag(_frags[frag_idx], pos_in_frag, src, len_to_fill);
            src += len_to_fill;
            position += len_to_fill;
        }
    }

    template<class _CharT>
    inline void basic_string_block<_CharT>::_copy_into_frag(
        const basic_string_pool_fragment<_CharT>& frag,
        size_t offset,
        const _CharT* src,
        size_t length) {
        std::memcpy(frag.begin() + offset, src, length);
    }
}

#endif // !LIBTEXT_STRING_BLOCK_H
