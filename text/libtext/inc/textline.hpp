#ifndef LIBTEXT_TEXT_LINE_H
#define LIBTEXT_TEXT_LINE_H

#include "stringblock.hpp"
#include "textmem.hpp"

namespace text {
    template<class _CharT>
    class basic_text_line {
    public:
        basic_text_line();
        explicit basic_text_line(const std::basic_string<_CharT>& initstr);
        explicit basic_text_line(const std::basic_string_view<_CharT>& initstr);
        template<class _StrLikeT>
        void append(const _StrLikeT& str);
        std::basic_string<_CharT> str() const;
        std::basic_string<_CharT> str(size_t offset, size_t len) const;
    private:
        basic_string_block<_CharT> _data;
    };

    using text_line = basic_text_line<char>;
    using u16text_line = basic_text_line<char16_t>;

    template<class _CharT>
    class basic_text_line_view {
    public:
        basic_text_line_view(const basic_text_line<_CharT>& origin, size_t offset, size_t len);
        std::basic_string<_CharT> str() const;
        size_t offset() const;
        size_t length() const;
    private:
        const basic_text_line<_CharT>& _origin;
        const size_t _offset;
        const size_t _len;
    };

    using text_line_view = basic_text_line_view<char>;
    using u16text_line_view = basic_text_line_view<char16_t>;

    // implementation -----------------------------------------------
    /**
     * @brief basic_text_line constructor 
     */
    template<class _CharT>
    inline basic_text_line<_CharT>::basic_text_line()
        : _data(make_string_block<_CharT>())
    {}

    /**
     * @brief basic_text_line constructor 
     */
    template<class _CharT>
    inline basic_text_line<_CharT>::basic_text_line(const std::basic_string<_CharT>& initstr)
        : _data(make_string_block<_CharT>(initstr))
    {}

    /**
     * @brief basic_text_line constructor 
     */
    template<class _CharT>
    inline basic_text_line<_CharT>::basic_text_line(const std::basic_string_view<_CharT>& initstr)
        : _data(make_string_block<_CharT>(initstr))
    {}

    /**
     * @brief append string at tail
     */
    template<class _CharT>
    template<class _StrLikeT>
    inline void basic_text_line<_CharT>::append(const _StrLikeT& str){
        _data.append(str);
    }

    /**
     * @brief get string
     */
    template<class _CharT>
    inline std::basic_string<_CharT> basic_text_line<_CharT>::str() const {
        return _data.str();
    }

    /**
     * @brief get string with offset and length
     */
    template<class _CharT>
    inline std::basic_string<_CharT> basic_text_line<_CharT>::str(size_t offset, size_t len) const {
        return _data.str(offset, len);
    }

    /**
     * @brief basic_text_line_view constructor
     */
    template<class _CharT>
    inline basic_text_line_view<_CharT>::basic_text_line_view(const basic_text_line<_CharT>& origin, size_t offset, size_t len)
        : _origin(origin)
        , _offset(offset)
        , _len(len)
    {}

    /**
     * @brief get string
     */
    template<class _CharT>
    inline std::basic_string<_CharT> basic_text_line_view<_CharT>::str() const {
        return _origin.str(_offset, _len);
    }

    template<class _CharT>
    inline size_t basic_text_line_view<_CharT>::offset() const {
        return _offset;
    }

    template<class _CharT>
    inline size_t basic_text_line_view<_CharT>::length() const {
        return _len;
    }
}

#endif // !LIBTEXT_TEXT_LINE_H
