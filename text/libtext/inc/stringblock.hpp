#ifndef LIBTEXT_STRING_BLOCK_H
#define LIBTEXT_STRING_BLOCK_H

#include <cassert>
#include <string>
#include "stringpool.hpp"

#ifdef min
# undef min // kill the fxxking evil macro.
#endif

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
        explicit basic_string_block(
            const basic_string_pool_allocate_fn<_CharT>& allocate,
            const basic_string_pool_deallocate_fn<_CharT>& deallocate,
            const basic_string_pool_fragsize_fn& fragsize,
            const std::basic_string_view<_CharT>& init_string);
        basic_string_block(const basic_string_block&) = delete;
        basic_string_block(basic_string_block&& rhs) noexcept;
        basic_string_block& operator=(const basic_string_block&) = delete;
        basic_string_block& operator=(basic_string_block&& rhs) noexcept;

        ~basic_string_block();
        template<class _StrLikeT> // TODO: C++20, use concept
        void append(const _StrLikeT& str);
        std::basic_string<_CharT> str() const;
        std::basic_string<_CharT> str(size_t offset, size_t len) const;
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

    /**
     * @brief constructor
     */
    template<class _CharT>
    inline basic_string_block<_CharT>::basic_string_block(
        const basic_string_pool_allocate_fn<_CharT>& allocate,
        const basic_string_pool_deallocate_fn<_CharT>& deallocate,
        const basic_string_pool_fragsize_fn& fragsize,
        const std::basic_string_view<_CharT>& init_string)
        : _length(0)
        , _allocate(allocate)
        , _deallocate(deallocate)
        , _fragsize(fragsize) {
        append(init_string);
    }

    /**
     * @brief move constructor
     */
    template<class _CharT>
    inline basic_string_block<_CharT>::basic_string_block(basic_string_block<_CharT>&& rhs) noexcept
        : _length(rhs._length)
        , _allocate(std::move(rhs._allocate))
        , _deallocate(std::move(rhs._deallocate))
        , _fragsize(std::move(rhs._fragsize))
        , _frags(std::move(rhs._frags)) {}

    /**
     * @brief move assignment
     */
    template<class _CharT>
    inline basic_string_block<_CharT>&
    basic_string_block<_CharT>::operator=(basic_string_block<_CharT>&& rhs) noexcept {
        _length = rhs._length;
        _allocate = std::move(rhs._allocate);
        _deallocate = std::move(rhs._deallocate);
        _fragsize = std::move(rhs._fragsize);
        _frags = std::move(rhs._frags);
        return this;
    }

    /**
     * @brief destructor
     */
    template<class _CharT>
    inline basic_string_block<_CharT>::~basic_string_block(){
        for (auto& frag : _frags) {
            _deallocate(std::move(frag));
        }
    }

    /**
     * @brief append string-like data to the block. 
     */
    template<class _CharT>
    template<class _StrLikeT>
    inline void basic_string_block<_CharT>::append(const _StrLikeT& str) {
        auto len_to_process = str.size();
        auto position = _length;
        _length += str.length();
        const _CharT* data = str.data();
        while (position < _length) {
            auto frag_idx = details::at_nth_frag(position, _fragsize());
            assert(frag_idx == _frags.size() - 1 || frag_idx == _frags.size());
            if (frag_idx == _frags.size()) {
                _frags.emplace_back(_allocate());
            }
            const auto len_available_in_frag = details::available_length_in_frag(position, _fragsize());
            const auto pos_in_frag = details::at_pos_in_frag(position, _fragsize());
            const auto len_to_fill = std::min(_length - position, _fragsize());
            _copy_into_frag(_frags[frag_idx], pos_in_frag, data, len_to_fill);
            data += len_to_fill;
            position += len_to_fill;
        }
    }

    /**
     * @brief fetch the whole string
     */
    template<class _CharT>
    inline std::basic_string<_CharT>
    basic_string_block<_CharT>::str() const {
        std::basic_string<_CharT> result;
        result.resize(_length);
        const auto fragsize = _fragsize();
        auto len_total = _length;
        auto base = result.data();
        for (const auto& frag : _frags) {
            auto len = std::min(len_total, fragsize);
            std::memcpy(base, frag.begin(), len);
            len_total -= len;
            base += len;
        }
        return result;
    }

    /**
     * @brief fetch string by specifying a range
     */
    template<class _CharT>
    inline std::basic_string<_CharT>
    basic_string_block<_CharT>::str(size_t offset, size_t len) const {
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
