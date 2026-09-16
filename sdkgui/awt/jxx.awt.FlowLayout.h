#pragma once

#include "awt/jxx.awt.LayoutManager.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class FlowLayout :
        public ::jxx::lang::ClassBase<FlowLayout, ::jxx::lang::Object, LayoutManager>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<FlowLayout, JxxSuper, LayoutManager>;

        static constexpr ::jxx::lang::jint LEFT = 0;
        static constexpr ::jxx::lang::jint CENTER = 1;
        static constexpr ::jxx::lang::jint RIGHT = 2;
        static constexpr ::jxx::lang::jint LEADING = 3;
        static constexpr ::jxx::lang::jint TRAILING = 4;

        FlowLayout();
        explicit FlowLayout(::jxx::lang::jint align);
        FlowLayout(::jxx::lang::jint align, ::jxx::lang::jint hgap, ::jxx::lang::jint vgap);

        ::jxx::lang::jint getAlignment() const;
        void setAlignment(::jxx::lang::jint align);
        ::jxx::lang::jint getHgap() const;
        void setHgap(::jxx::lang::jint hgap);
        ::jxx::lang::jint getVgap() const;
        void setVgap(::jxx::lang::jint vgap);
        ::jxx::lang::jbool getAlignOnBaseline() const;
        void setAlignOnBaseline(::jxx::lang::jbool alignOnBaseline);

        void addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<Component>& component) override;
        void removeLayoutComponent(const ::jxx::Ptr<Component>& component) override;
        ::jxx::Ptr<Dimension> preferredLayoutSize(const ::jxx::Ptr<Container>& parent) override;
        ::jxx::Ptr<Dimension> minimumLayoutSize(const ::jxx::Ptr<Container>& parent) override;
        void layoutContainer(const ::jxx::Ptr<Container>& parent) override;

    private:
        ::jxx::lang::jint align_;
        ::jxx::lang::jint hgap_;
        ::jxx::lang::jint vgap_;
        ::jxx::lang::jbool alignOnBaseline_;
    };
}
