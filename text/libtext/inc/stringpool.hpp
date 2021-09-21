#ifndef LIBTEXT_STRING_POOL_H
#define LIBTEXT_STRING_POOL_H

#include <cstdint>
#include <vector>
#include <tuple>

namespace text {
    template<size_t _SizeFrag, class _CharT>
    class basic_string_pool {
    public:
        static inline constexpr const size_t fragment_size = _SizeFrag;
        struct fragment_t {
            size_t index;
            _CharT* begin;
            _CharT* end;
        };

        fragment_t allocate();
        void deallocate(size_t index);
        void shrink(); // TODO

    private:
        enum struct FragStatus
        {
            Free = 0,
            Occupied = 1
        };
        std::vector<FragStatus> _frag_status;
        std::vector<std::array<_CharT, _SizeFrag>> _frag_pool;
    };

    using string_pool = basic_string_pool <1024, char>;
    using wstring_pool = basic_string_pool <1024, char16_t>;

    // implementations ----------------------------------------------
    /**
     * @brief allocate
     */
    template<size_t _SizeFrag, class _CharT>
    inline typename basic_string_pool <_SizeFrag, _CharT>::fragment_t
    basic_string_pool <_SizeFrag, _CharT>::allocate() {
        fragment_t frag{ 0 };
        size_t idx = 0;
        while (idx < _frag_status.size() && _frag_status[idx] == FragStatus::Occupied) {
            ++idx;
        }
        if (idx < _frag_status.size()) { // found an available slot
            frag.index = idx;
            frag.begin = _frag_pool[idx].data();
            frag.end = frag.begin + _SizeFrag;
            _frag_status[idx] = FragStatus::Occupied;
        }
        else { // the pool needs more space
            _frag_status.emplace_back(FragStatus::Occupied);
            _frag_pool.emplace_back(std::array<_CharT, _SizeFrag>());
            frag.index = idx;
            frag.begin = _frag_pool[idx].data();
            frag.end = frag.begin + _SizeFrag;
        }
        return frag;
    }

    /**
     * @brief deallocate
     */
    template<size_t _SizeFrag, class _CharT>
    inline void
    basic_string_pool <_SizeFrag, _CharT>::deallocate(size_t index) {
        _frag_status[index] = FragStatus::Free;
    }
}

#endif // !LIBTEXT_STRING_POOL_H
