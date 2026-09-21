#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt
{
    class Font;
    class Graphics;

    class FontMetrics : public ::jxx::lang::ClassBase<FontMetrics,
        ::jxx::lang::Object>
    {
    public:
        using JxxSuper=::jxx::lang::Object;
        using Super=::jxx::lang::ClassBase<FontMetrics,JxxSuper>;
        using WidthArray=::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1>>;
        ~FontMetrics() override=default;

        ::jxx::Ptr<Font> getFont()const;
        virtual ::jxx::lang::jint getLeading()const;
        virtual ::jxx::lang::jint getAscent()const;
        virtual ::jxx::lang::jint getDescent()const;
        virtual ::jxx::lang::jint getHeight()const;
        virtual ::jxx::lang::jint getMaxAscent()const;
        virtual ::jxx::lang::jint getMaxDescent()const;
        virtual ::jxx::lang::jint charWidth(::jxx::lang::jchar character)const;
        virtual ::jxx::lang::jint stringWidth(
            const ::jxx::Ptr<::jxx::lang::String>& text)const;
        virtual WidthArray getWidths()const;
        ::jxx::lang::jbool hasUniformLineMetrics()const;
        ::jxx::Ptr<::jxx::lang::String> toString()const override;

    protected:
        explicit FontMetrics(const ::jxx::Ptr<Font>& font);

    private:
        ::jxx::Ptr<Font> font_;
    };
}
