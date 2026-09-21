#pragma once

#include "awt/jxx.awt.Container.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing
{
    class JComponent;

    class LayoutStyle : public ::jxx::lang::ClassBase<
        LayoutStyle, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<LayoutStyle, JxxSuper>;

        enum class ComponentPlacement
        {
            RELATED,
            UNRELATED,
            INDENT
        };

        LayoutStyle();
        ~LayoutStyle() override = default;

        static void setInstance(const ::jxx::Ptr<LayoutStyle>& style);
        static ::jxx::Ptr<LayoutStyle> getInstance();

        virtual ::jxx::lang::jint getPreferredGap(
            const ::jxx::Ptr<JComponent>& component1,
            const ::jxx::Ptr<JComponent>& component2,
            ComponentPlacement type,
            ::jxx::lang::jint position,
            const ::jxx::Ptr<::jxx::awt::Container>& parent) = 0;
        virtual ::jxx::lang::jint getContainerGap(
            const ::jxx::Ptr<JComponent>& component,
            ::jxx::lang::jint position,
            const ::jxx::Ptr<::jxx::awt::Container>& parent) = 0;
    };
}
