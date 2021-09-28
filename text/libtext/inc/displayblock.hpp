#ifndef LIBTEXT_DISPLAY_BLOCK_H
#define LIBTEXT_DISPLAY_BLOCK_H

#include <vector>
#include "textdoc.hpp"
#include "textline.hpp"

namespace text {
    class screen_window {
        int32_t x;
        int32_t y;
        int32_t w;
        int32_t h;
    };

    template<class _CharT>
    class display_block {
    public:
        struct line {
            screen_window window;
            basic_text_line<_CharT> content;
        };

    private:
        std::vector<line> _lines;
    };

    template<class _CharT>
    class display_block_evaluator {
    public:
        display_block<_CharT> evaluate(const screen_window& window, const basic_text_doc<_CharT>& doc);

    private:
    };
}

#endif // !LIBTEXT_DISPLAY_BLOCK_H
