#ifndef LIBTEXT_STRING_POOL_H
#define LIBTEXT_STRING_POOL_H

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace text {
    struct string_pool_error : public std::runtime_error {
        string_pool_error() : std::runtime_error("string pool error") {}
        explicit string_pool_error(const char* msg) : std::runtime_error(msg) {}
        string_pool_error(const string_pool_error&) = delete;
        string_pool_error(string_pool_error&&) = default;
    };

    template<class _CharT>
    struct basic_string_pool_fragment {
        size_t index;
        std::function<_CharT*()> begin;
        std::function<_CharT*()> end;
    };

    template<size_t _SizeFrag, class _CharT>
    class basic_string_pool {
    public:
        static inline constexpr const size_t fragment_size = _SizeFrag;
        explicit basic_string_pool(size_t fragment_count);
        basic_string_pool_fragment<_CharT> allocate();
        void deallocate(basic_string_pool_fragment<_CharT>&& fragment);

    private:
        enum struct FragStatus
        {
            Free = 0,
            Occupied = 1
        };
        std::vector<FragStatus> _frag_status;
        std::vector<std::array<_CharT, _SizeFrag>> _frag_pool;
    };

    using basic_string_pool_fragsize_fn = std::function<size_t()>;
    template<class _CharT>
    using basic_string_pool_allocate_fn = std::function<basic_string_pool_fragment<_CharT>()>;
    template<class _CharT>
    using basic_string_pool_deallocate_fn = std::function<void(basic_string_pool_fragment<_CharT>&&)>;

    using string_pool = basic_string_pool <1024, char>;
    using wstring_pool = basic_string_pool <1024, char16_t>;

    // implementations ----------------------------------------------
    /**
     * @brief basic_string_pool 
     */
    template<size_t _SizeFrag, class _CharT>
    inline basic_string_pool <_SizeFrag, _CharT>::basic_string_pool(size_t fragment_count) {
        _frag_status.resize(fragment_count);
        _frag_pool.resize(fragment_count);
    }

    /**
     * @brief allocate
     */
    template<size_t _SizeFrag, class _CharT>
    inline basic_string_pool_fragment<_CharT>
    basic_string_pool <_SizeFrag, _CharT>::allocate() {
        basic_string_pool_fragment<_CharT> frag{ 0 };
        size_t idx = 0;
        while (idx < _frag_status.size() && _frag_status[idx] == FragStatus::Occupied) {
            ++idx;
        }
        if (idx < _frag_status.size()) { // found an available slot
            frag.index = idx;
            frag.begin = [this, idx]() { return _frag_pool[idx].data(); };
            frag.end = [this, idx]() { return _frag_pool[idx].data() + fragment_size; };
            _frag_status[idx] = FragStatus::Occupied;
        }
        else {
            throw string_pool_error("string pool error: running out of fragment resources.");
        }
        return frag;
    }

    /**
     * @brief deallocate
     */
    template<size_t _SizeFrag, class _CharT>
    inline void
    basic_string_pool <_SizeFrag, _CharT>::deallocate(basic_string_pool_fragment<_CharT>&& fragment) {
        _frag_status[fragment.index] = FragStatus::Free;
    }
}

#endif // !LIBTEXT_STRING_POOL_H
