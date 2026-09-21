#pragma once

#include <array>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt
{
    class PageAttributes final : public ::jxx::lang::ClassBase<
        PageAttributes, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<PageAttributes, JxxSuper>;

        enum class ColorType { COLOR, MONOCHROME };
        enum class MediaType { NA_LETTER, NA_LEGAL, EXECUTIVE, ISO_A4 };
        enum class OrientationRequestedType { PORTRAIT, LANDSCAPE };
        enum class OriginType { PHYSICAL, PRINTABLE };
        enum class PrintQualityType { HIGH, NORMAL, DRAFT };
        using ResolutionArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::lang::jint, 1>>;

        PageAttributes();
        PageAttributes(ColorType color, MediaType media,
            OrientationRequestedType orientation, OriginType origin,
            PrintQualityType quality,
            const ResolutionArray& printerResolution);
        explicit PageAttributes(const ::jxx::Ptr<PageAttributes>& source);

        ColorType getColor() const; void setColor(ColorType value);
        MediaType getMedia() const; void setMedia(MediaType value);
        OrientationRequestedType getOrientationRequested() const;
        void setOrientationRequested(OrientationRequestedType value);
        OriginType getOrigin() const; void setOrigin(OriginType value);
        PrintQualityType getPrintQuality() const;
        void setPrintQuality(PrintQualityType value);
        ResolutionArray getPrinterResolution() const;
        void setPrinterResolution(const ResolutionArray& value);
        void set(const ::jxx::Ptr<PageAttributes>& source);
        ::jxx::Ptr<::jxx::lang::Object> clone() const override;
        ::jxx::lang::jbool equals(
            const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
        ::jxx::lang::jint hashCode() const override;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    private:
        ColorType color_ = ColorType::MONOCHROME;
        MediaType media_ = MediaType::NA_LETTER;
        OrientationRequestedType orientation_ =
            OrientationRequestedType::PORTRAIT;
        OriginType origin_ = OriginType::PHYSICAL;
        PrintQualityType quality_ = PrintQualityType::NORMAL;
        std::array<::jxx::lang::jint, 3> resolution_{{72,72,3}};
    };
}
