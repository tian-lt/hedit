#ifndef LIBTEXT_TEXT_LINE_H
#define LIBTEXT_TEXT_LINE_H

#include "stringblock.hpp"
#include "textmem.hpp"

namespace text {
    template<class _CharT>
    class basic_text_line {
    public:
        basic_text_line();
        explicit basic_text_line(const std::string& initstr);

    private:
        basic_string_block<_CharT> _data;
    };

    using text_line = basic_text_line<char>;
    using wtext_line = basic_text_line<char16_t>;

    // implementation -----------------------------------------------
    /**
     * basic_text_line
     */
    template<class _CharT>
    basic_text_line<_CharT>::basic_text_line()
        : _data(make_string_block<_CharT>())
    {}

    /**
     * basic_text_line
     */
    template<class _CharT>
    basic_text_line<_CharT>::basic_text_line(const std::string& initstr)
        : _data(make_string_block<_CharT>(initstr))
    {}
}

#endif // !LIBTEXT_TEXT_LINE_H
