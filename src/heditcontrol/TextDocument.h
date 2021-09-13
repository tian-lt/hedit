#pragma once

namespace HeditControls
{
    public ref class TextRange sealed
    {
    public:
        property size_t Begin
        {
            size_t get() { return m_beg; }
            void set(size_t value) { m_beg = value; }
        }
        property size_t End
        {
            size_t get() { return m_beg; }
            void set(size_t value) { m_end = value; }
        }

    private:
        size_t m_beg;
        size_t m_end;
    };

    public interface class ITextDocument
    {
        void SetText(Platform::String^ text);
        Platform::String^ GetText();
        Platform::String^ GetText(TextRange^ range);
    };

    public delegate void TextChangedDelegate();

    public ref class StaticTextDocument sealed : public ITextDocument
    {
    public:
        explicit StaticTextDocument(TextChangedDelegate^ changedDelegate);

    public:
        property Platform::String^ Text
        {
            Platform::String^ get() { return m_text; }
            void set(Platform::String^ value) { SetText(value); }
        }

    public:
        virtual void SetText(Platform::String^ text);
        virtual Platform::String^ GetText();
        virtual Platform::String^ GetText(TextRange^ range);

    private:
        Platform::String^ m_text;
        TextChangedDelegate^ m_changedCB;
    };
}

