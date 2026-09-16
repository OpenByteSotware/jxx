#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util
{
    class Locale;
    class ResourceBundle;
}

namespace jxx::awt
{
    class ComponentOrientation final :
        public ::jxx::lang::ClassBase<ComponentOrientation,
            ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ComponentOrientation, JxxSuper>;

        static ::jxx::Ptr<ComponentOrientation> LEFT_TO_RIGHT;
        static ::jxx::Ptr<ComponentOrientation> RIGHT_TO_LEFT;
        static ::jxx::Ptr<ComponentOrientation> UNKNOWN;

        ::jxx::lang::jbool isHorizontal() const;
        ::jxx::lang::jbool isLeftToRight() const;

        static ::jxx::Ptr<ComponentOrientation> getOrientation(
            const ::jxx::Ptr<::jxx::util::Locale>& locale);

        static ::jxx::Ptr<ComponentOrientation> getOrientation(
            const ::jxx::Ptr<::jxx::util::ResourceBundle>& bundle);

    private:
        static ::jxx::Ptr<ComponentOrientation> create(
            ::jxx::lang::jbool horizontal,
            ::jxx::lang::jbool leftToRight);

        ComponentOrientation(::jxx::lang::jbool horizontal,
            ::jxx::lang::jbool leftToRight);

        ::jxx::lang::jbool horizontal_;
        ::jxx::lang::jbool leftToRight_;
    };
}
