#ifndef LIBTEXT_DISPLAY_BLOCK_H
#define LIBTEXT_DISPLAY_BLOCK_H

#include <functional>
#include <vector>
#include "textdoc.hpp"
#include "textline.hpp"

namespace text {
    class rect {
        int32_t x;
        int32_t y;
        int32_t w;
        int32_t h;
    };

    template<class _CharT>
    class display_block {
    public:
        struct line {
            rect window;
            basic_text_line_view<_CharT> content;
        };

    private:
        std::vector<line> _lines;
    };

    template<class _CharT, class _MatResolver>
    class display_block_evaluator {
    public:
        display_block<_CharT> evaluate(
            const rect& window,
            size_t beg_line,
            size_t beg_col,
            const basic_text_doc<_CharT>& doc);
    };

    template<class _CharT, class _MatResolver>
    inline display_block<_CharT> display_block_evaluator<_CharT, _MatResolver>::evaluate(
        const rect& window,
        size_t beg_line,
        size_t beg_col,
        const basic_text_doc<_CharT>& doc) {
    }
}

#endif // !LIBTEXT_DISPLAY_BLOCK_H
