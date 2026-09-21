#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class Dimension;
    class EventQueue;
    class Font;
    class FontMetrics;
    class Image;

    class Toolkit : public ::jxx::lang::ClassBase<Toolkit,
        ::jxx::lang::Object>
    {
    public:
        using JxxSuper=::jxx::lang::Object;
        using Super=::jxx::lang::ClassBase<Toolkit,JxxSuper>;
        ~Toolkit() override=default;

        static ::jxx::Ptr<Toolkit> getDefaultToolkit();
        static ::jxx::Ptr<::jxx::lang::String> getProperty(
            const ::jxx::Ptr<::jxx::lang::String>& key,
            const ::jxx::Ptr<::jxx::lang::String>& defaultValue);
        virtual ::jxx::Ptr<Dimension> getScreenSize()const=0;
        virtual ::jxx::lang::jint getScreenResolution()const=0;
        virtual ::jxx::Ptr<FontMetrics> getFontMetrics(
            const ::jxx::Ptr<Font>& font)const=0;
        virtual void sync()=0;
        virtual void beep()=0;
        virtual ::jxx::Ptr<EventQueue> getSystemEventQueue()const;
        virtual ::jxx::lang::jbool areExtraMouseButtonsEnabled()const;

    protected:
        Toolkit();
    };
}
