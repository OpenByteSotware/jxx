#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class DisplayMode final :
        public ::jxx::lang::ClassBase<DisplayMode, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<DisplayMode, JxxSuper>;

        static constexpr ::jxx::lang::jint BIT_DEPTH_MULTI = -1;
        static constexpr ::jxx::lang::jint REFRESH_RATE_UNKNOWN = 0;

        DisplayMode(::jxx::lang::jint width,
            ::jxx::lang::jint height,
            ::jxx::lang::jint bitDepth,
            ::jxx::lang::jint refreshRate);

        ::jxx::lang::jint getWidth() const;
        ::jxx::lang::jint getHeight() const;
        ::jxx::lang::jint getBitDepth() const;
        ::jxx::lang::jint getRefreshRate() const;
        ::jxx::lang::jbool equals(const ::jxx::Ptr<DisplayMode>& mode) const;
        ::jxx::lang::jbool equals(
            const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
        ::jxx::lang::jint hashCode() const override;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    private:
        ::jxx::lang::jint width_;
        ::jxx::lang::jint height_;
        ::jxx::lang::jint bitDepth_;
        ::jxx::lang::jint refreshRate_;
    };
}
