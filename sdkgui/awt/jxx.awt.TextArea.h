#pragma once

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.TextComponent.h"

namespace jxx::awt
{
    class TextArea : public ::jxx::lang::ClassBase<TextArea, TextComponent>
    {
    public:
        using JxxSuper = TextComponent;
        using Super = ::jxx::lang::ClassBase<TextArea, JxxSuper>;

        static constexpr ::jxx::lang::jint SCROLLBARS_BOTH = 0;
        static constexpr ::jxx::lang::jint SCROLLBARS_VERTICAL_ONLY = 1;
        static constexpr ::jxx::lang::jint SCROLLBARS_HORIZONTAL_ONLY = 2;
        static constexpr ::jxx::lang::jint SCROLLBARS_NONE = 3;

        TextArea();
        explicit TextArea(const ::jxx::Ptr<::jxx::lang::String>& text);
        TextArea(::jxx::lang::jint rows, ::jxx::lang::jint columns);
        TextArea(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint rows, ::jxx::lang::jint columns);
        TextArea(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint rows, ::jxx::lang::jint columns,
            ::jxx::lang::jint scrollbars);
        ~TextArea() override = default;

        void insert(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint position);
        void insertText(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint position);
        void append(const ::jxx::Ptr<::jxx::lang::String>& text);
        void appendText(const ::jxx::Ptr<::jxx::lang::String>& text);
        void replaceRange(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint start, ::jxx::lang::jint end);
        void replaceText(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint start, ::jxx::lang::jint end);
        ::jxx::lang::jint getRows() const;
        void setRows(::jxx::lang::jint rows);
        ::jxx::lang::jint getColumns() const;
        void setColumns(::jxx::lang::jint columns);
        ::jxx::lang::jint getScrollbarVisibility() const;
        ::jxx::Ptr<Dimension> getPreferredSize(::jxx::lang::jint rows,
            ::jxx::lang::jint columns) const;
        ::jxx::Ptr<Dimension> preferredSize(::jxx::lang::jint rows,
            ::jxx::lang::jint columns) const;
        ::jxx::Ptr<Dimension> getMinimumSize(::jxx::lang::jint rows,
            ::jxx::lang::jint columns) const;
        ::jxx::Ptr<Dimension> minimumSize(::jxx::lang::jint rows,
            ::jxx::lang::jint columns) const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        ::jxx::lang::jint rows_;
        ::jxx::lang::jint columns_;
        ::jxx::lang::jint scrollbarVisibility_;
    };
}
