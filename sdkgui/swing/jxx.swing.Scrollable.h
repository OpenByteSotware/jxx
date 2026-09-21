#pragma once

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Rectangle.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::swing
{
    class Scrollable : public ::jxx::lang::InterfaceBase<Scrollable>
    {
    public:
        ~Scrollable() override = default;

        virtual ::jxx::Ptr<::jxx::awt::Dimension>
            getPreferredScrollableViewportSize() const = 0;

        virtual ::jxx::lang::jint getScrollableUnitIncrement(
            const ::jxx::Ptr<::jxx::awt::Rectangle>& visibleRect,
            ::jxx::lang::jint orientation,
            ::jxx::lang::jint direction) const = 0;

        virtual ::jxx::lang::jint getScrollableBlockIncrement(
            const ::jxx::Ptr<::jxx::awt::Rectangle>& visibleRect,
            ::jxx::lang::jint orientation,
            ::jxx::lang::jint direction) const = 0;

        virtual ::jxx::lang::jbool
            getScrollableTracksViewportWidth() const = 0;

        virtual ::jxx::lang::jbool
            getScrollableTracksViewportHeight() const = 0;
    };
}
