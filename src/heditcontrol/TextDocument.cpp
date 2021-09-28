#include "pch.h"

#include "text.hpp"
#include "TextDocument.h"

using namespace HeditControls;
StaticTextDocument::StaticTextDocument(TextChangedDelegate^ textChangedDelegate)
{
    m_changedCB = textChangedDelegate;
}

void StaticTextDocument::SetText(Platform::String^ text)
{
    m_text = text;
    if (m_changedCB != nullptr) {
        m_changedCB->Invoke();
    }
}

Platform::String^ StaticTextDocument::GetText()
{
    return m_text;
}

Platform::String^ StaticTextDocument::GetText(TextRange^ range)
{
    return ref new Platform::String(
        m_text->Begin() + range->Begin,
        range->End - range->Begin);
}



