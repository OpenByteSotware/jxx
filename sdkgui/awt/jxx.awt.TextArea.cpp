#include "awt/jxx.awt.TextArea.h"

#include <algorithm>
#include <sstream>
#include <string>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> emptyText()
        {
            return ::jxx::NEW<::jxx::lang::String>("");
        }
    }

    TextArea::TextArea() : TextArea(emptyText(), 0, 0, SCROLLBARS_BOTH) {}
    TextArea::TextArea(const ::jxx::Ptr<::jxx::lang::String>& text)
        : TextArea(text, 0, 0, SCROLLBARS_BOTH) {}
    TextArea::TextArea(::jxx::lang::jint rows, ::jxx::lang::jint columns)
        : TextArea(emptyText(), rows, columns, SCROLLBARS_BOTH) {}
    TextArea::TextArea(const ::jxx::Ptr<::jxx::lang::String>& text,
        ::jxx::lang::jint rows, ::jxx::lang::jint columns)
        : TextArea(text, rows, columns, SCROLLBARS_BOTH) {}

    TextArea::TextArea(const ::jxx::Ptr<::jxx::lang::String>& text,
        ::jxx::lang::jint rows, ::jxx::lang::jint columns,
        ::jxx::lang::jint scrollbars)
        : Super(text != nullptr ? text : emptyText()),
          rows_(std::max<::jxx::lang::jint>(0, rows)),
          columns_(std::max<::jxx::lang::jint>(0, columns)),
          scrollbarVisibility_(scrollbars)
    {
        if (scrollbars < SCROLLBARS_BOTH || scrollbars > SCROLLBARS_NONE)
            throw ::jxx::lang::IllegalArgumentException("scrollbars");
    }

    void TextArea::insert(const ::jxx::Ptr<::jxx::lang::String>& value,
        ::jxx::lang::jint position)
    {
        auto current = getText()->utf8();
        if (position < 0 || position > static_cast<::jxx::lang::jint>(current.size()))
            throw ::jxx::lang::IllegalArgumentException("position");
        current.insert(static_cast<std::size_t>(position),
            value != nullptr ? value->utf8() : std::string());
        setText(::jxx::NEW<::jxx::lang::String>(current));
    }
    void TextArea::insertText(const ::jxx::Ptr<::jxx::lang::String>& value,
        ::jxx::lang::jint position) { insert(value, position); }
    void TextArea::append(const ::jxx::Ptr<::jxx::lang::String>& value)
    {
        insert(value, static_cast<::jxx::lang::jint>(getText()->utf8().size()));
    }
    void TextArea::appendText(const ::jxx::Ptr<::jxx::lang::String>& value)
    {
        append(value);
    }
    void TextArea::replaceRange(const ::jxx::Ptr<::jxx::lang::String>& value,
        ::jxx::lang::jint start, ::jxx::lang::jint end)
    {
        auto current = getText()->utf8();
        if (start < 0 || end < start ||
            end > static_cast<::jxx::lang::jint>(current.size()))
            throw ::jxx::lang::IllegalArgumentException("range");
        current.replace(static_cast<std::size_t>(start),
            static_cast<std::size_t>(end - start),
            value != nullptr ? value->utf8() : std::string());
        setText(::jxx::NEW<::jxx::lang::String>(current));
    }
    void TextArea::replaceText(const ::jxx::Ptr<::jxx::lang::String>& value,
        ::jxx::lang::jint start, ::jxx::lang::jint end)
    {
        replaceRange(value, start, end);
    }
    ::jxx::lang::jint TextArea::getRows() const { return rows_; }
    void TextArea::setRows(::jxx::lang::jint rows)
    {
        if (rows < 0) throw ::jxx::lang::IllegalArgumentException("rows");
        rows_ = rows; invalidate();
    }
    ::jxx::lang::jint TextArea::getColumns() const { return columns_; }
    void TextArea::setColumns(::jxx::lang::jint columns)
    {
        if (columns < 0) throw ::jxx::lang::IllegalArgumentException("columns");
        columns_ = columns; invalidate();
    }
    ::jxx::lang::jint TextArea::getScrollbarVisibility() const
    {
        return scrollbarVisibility_;
    }
    ::jxx::Ptr<Dimension> TextArea::getPreferredSize(
        ::jxx::lang::jint rows, ::jxx::lang::jint columns) const
    {
        return ::jxx::NEW<Dimension>(std::max(0, columns) * 8 + 16,
            std::max(0, rows) * 16 + 16);
    }
    ::jxx::Ptr<Dimension> TextArea::preferredSize(
        ::jxx::lang::jint rows, ::jxx::lang::jint columns) const
    {
        return getPreferredSize(rows, columns);
    }
    ::jxx::Ptr<Dimension> TextArea::getMinimumSize(
        ::jxx::lang::jint rows, ::jxx::lang::jint columns) const
    {
        return getPreferredSize(rows, columns);
    }
    ::jxx::Ptr<Dimension> TextArea::minimumSize(
        ::jxx::lang::jint rows, ::jxx::lang::jint columns) const
    {
        return getMinimumSize(rows, columns);
    }
    ::jxx::Ptr<::jxx::lang::String> TextArea::paramString() const
    {
        std::ostringstream out;
        out << "rows=" << rows_ << ",columns=" << columns_
            << ",scrollbarVisibility=" << scrollbarVisibility_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
