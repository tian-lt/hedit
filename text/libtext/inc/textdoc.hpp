#ifndef LIBTEXT_TEXT_DOC_H
#define LIBTEXT_TEXT_DOC_H

#include <vector>
#include "textline.hpp"

namespace text {
    namespace details {
        template<class _CharT>
        inline constexpr _CharT determine_line_end_sym();
        template<>
        inline constexpr char determine_line_end_sym<char>() { return '\n'; };
        template<>
        inline constexpr char16_t determine_line_end_sym<char16_t>() { return u'\n'; };
        template<>
        inline constexpr char32_t determine_line_end_sym<char32_t>() { return U'\n'; };
    }

    template<class _CharT>
    class basic_text_doc {
    public:
        static inline constexpr const _CharT endl = details::determine_line_end_sym<_CharT>();
    public:
        void append(const std::basic_string<_CharT>& str);
        size_t line_count() const { return _lines.size(); }
        std::basic_string<_CharT> str() const;
        const basic_text_line<_CharT>& operator[](size_t line) const;

    private:
        basic_text_line<_CharT>& _newline();
        basic_text_line<_CharT>& _ensure_lastline();

    private:
        std::vector<basic_text_line<_CharT>> _lines;
    };

    using text_doc = basic_text_doc<char>;
    using wtext_doc = basic_text_doc<char16_t>;

    // implementation -----------------------------------------------
    /**
     * append
     */
    template<class _CharT>
    inline void basic_text_doc<_CharT>::append(const std::basic_string<_CharT>& str) {
        basic_text_line<_CharT>* line = &_ensure_lastline();
        const _CharT* beg = str.data();
        size_t len = 0;
        for (const auto& c : str) {
            if (c != endl) {
                ++len;
            }
            else {
                line->append(std::basic_string_view<_CharT>(beg, len));
                beg += len + 1; // skip the line-end
                len = 0;
                line = &_newline();
            }
        }
        if (len > 0) {
            line->append(std::basic_string_view<_CharT>(beg, len));
        }
    }

    /*
     * @brief fetch the whole text from document
     */
    template<class _CharT>
    inline std::basic_string<_CharT> basic_text_doc<_CharT>::str() const {
        std::basic_string<_CharT> result;
        size_t i = 0;
        for (const auto& line : _lines) {
            result.append(line.str());
            if (i != _lines.size() - 1) {
                result.push_back(endl);
            }
            ++i;
        }
        return result;
    }

    /*
     * @brief access each line by line-index
     */
    template<class _CharT>
    inline const basic_text_line<_CharT>& basic_text_doc<_CharT>::operator[](size_t line) const {
        return _lines[line];
    }

    template<class _CharT>
    inline basic_text_line<_CharT>& basic_text_doc<_CharT>::_newline() {
        _lines.emplace_back();
        return _lines.back();
    }

    template<class _CharT>
    inline basic_text_line<_CharT>& basic_text_doc<_CharT>::_ensure_lastline() {
        return _lines.size() > 0 ? _lines.back() : _newline();
    }

}

#endif // !LIBTEXT_TEXT_DOC_H
