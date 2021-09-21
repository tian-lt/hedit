#ifndef LIBTEXT_STRING_BLOCK_H
#define LIBTEXT_STRING_BLOCK_H

#include <cassert>
#include <string>
#include "stringpool.hpp"

namespace text {
    namespace details {
        template<size_t _SizeFrag>
        inline constexpr size_t at_nth_frag(size_t position) { return position / _SizeFrag; }
        template<size_t _SizeFrag>
        inline constexpr size_t at_pos_in_frag(size_t position) { return position % _SizeFrag; }
        template<size_t _SizeFrag>
        inline constexpr size_t available_length_in_frag(size_t position) { return _SizeFrag - at_pos_in_frag<_SizeFrag>(position); }
    }

    template<class _CharT, size_t _SizeFrag>
    class basic_string_block {
    public:
        explicit basic_string_block(basic_string_pool<_SizeFrag, _CharT>* pool_ref);
        explicit basic_string_block(basic_string_pool<_SizeFrag, _CharT>* pool_ref, const std::basic_string<_CharT>& init_string);
        void append(const std::basic_string<_CharT>& src_str);
    private:
        using pool_t = basic_string_pool<_SizeFrag, _CharT>;
        static inline void _copy_into_frag(typename pool_t::fragment_t& frag, size_t offset, const _CharT* src, size_t length);
    private:
        static inline constexpr const size_t _frag_size = _SizeFrag;
        size_t _length;
        std::vector<typename pool_t::fragment_t> _frags;
        pool_t* _pool_ref;
    };

    using string_block = basic_string_block<char, 1024>;
    using wstring_block = basic_string_block<char16_t, 1024>;

    // implementations ----------------------------------------------
    /**
     * @brief constructor 
     */
    template<class _CharT, size_t _SizeFrag>
    inline basic_string_block<_CharT, _SizeFrag>::basic_string_block(basic_string_pool<_SizeFrag, _CharT>* pool_ref)
        : _length(0)
        , _pool_ref(pool_ref)
    {}

    /**
     * @brief constructor 
     */
    template<class _CharT, size_t _SizeFrag>
    inline basic_string_block<_CharT, _SizeFrag>::basic_string_block(
        basic_string_pool<_SizeFrag, _CharT>* pool_ref,
        const std::basic_string<_CharT>& init_string)
        : _length(0)
        , _pool_ref(pool_ref) {
        append(init_string);
    }

    template<class _CharT, size_t _SizeFrag>
    inline void basic_string_block<_CharT, _SizeFrag>::append(const std::basic_string<_CharT>& src_str) {
        auto len_to_process = src_str.size();
        auto position = _length;
        _length += src_str.length();
        const _CharT* src = src_str.data();
        while (position < _length) {
            auto frag_idx = details::at_nth_frag<_SizeFrag>(position);
            assert(frag_idx == _frags.size() - 1 || frag_idx == _frags.size());
            if (frag_idx == _frags.size()) {
                _frags.emplace_back(_pool_ref->allocate());
            }
            const auto len_available_in_frag = details::available_length_in_frag<_SizeFrag>(position);
            const auto pos_in_frag = details::at_pos_in_frag<_SizeFrag>(position);
            const auto len_to_fill = std::min(_length - position, _SizeFrag);
            _copy_into_frag(_frags[frag_idx], pos_in_frag, src, len_to_fill);
            src += len_to_fill;
            position += len_to_fill;
        }
    }

    template<class _CharT, size_t _SizeFrag>
    inline void basic_string_block<_CharT, _SizeFrag>::_copy_into_frag(typename pool_t::fragment_t& frag, size_t offset, const _CharT* src, size_t length) {
        std::memcpy(frag.begin + offset, src, length);
    }
}

#endif // !LIBTEXT_STRING_BLOCK_H
