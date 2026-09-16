#include "awt/jxx.awt.FlowLayout.h"

#include <algorithm>
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    FlowLayout::FlowLayout() : FlowLayout(CENTER, 5, 5) {}
    FlowLayout::FlowLayout(::jxx::lang::jint align) : FlowLayout(align, 5, 5) {}
    FlowLayout::FlowLayout(::jxx::lang::jint align, ::jxx::lang::jint hgap, ::jxx::lang::jint vgap)
        : align_(CENTER), hgap_(hgap), vgap_(vgap), alignOnBaseline_(false)
    {
        setAlignment(align);
    }

    ::jxx::lang::jint FlowLayout::getAlignment() const { return align_; }
    void FlowLayout::setAlignment(::jxx::lang::jint align)
    {
        if (align < LEFT || align > TRAILING)
            throw ::jxx::lang::IllegalArgumentException("invalid alignment");
        align_ = align;
    }
    ::jxx::lang::jint FlowLayout::getHgap() const { return hgap_; }
    void FlowLayout::setHgap(::jxx::lang::jint hgap) { hgap_ = hgap; }
    ::jxx::lang::jint FlowLayout::getVgap() const { return vgap_; }
    void FlowLayout::setVgap(::jxx::lang::jint vgap) { vgap_ = vgap; }
    ::jxx::lang::jbool FlowLayout::getAlignOnBaseline() const { return alignOnBaseline_; }
    void FlowLayout::setAlignOnBaseline(::jxx::lang::jbool value) { alignOnBaseline_ = value; }
    void FlowLayout::addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>&, const ::jxx::Ptr<Component>&) {}
    void FlowLayout::removeLayoutComponent(const ::jxx::Ptr<Component>&) {}

    ::jxx::Ptr<Dimension> FlowLayout::preferredLayoutSize(const ::jxx::Ptr<Container>& parent)
    {
        ::jxx::lang::jint width = hgap_;
        ::jxx::lang::jint height = 0;
        if (parent)
        {
            for (::jxx::lang::jint i = 0; i < parent->getComponentCount(); ++i)
            {
                auto component = parent->getComponent(i);
                if (!component || !component->isVisible()) continue;
                width += component->getWidth() + hgap_;
                height = std::max(height, component->getHeight());
            }
        }
        return ::jxx::NEW<Dimension>(width, height + 2 * vgap_);
    }

    ::jxx::Ptr<Dimension> FlowLayout::minimumLayoutSize(const ::jxx::Ptr<Container>& parent)
    {
        return preferredLayoutSize(parent);
    }

    void FlowLayout::layoutContainer(const ::jxx::Ptr<Container>& parent)
    {
        if (!parent) return;
        ::jxx::lang::jint total = 0;
        ::jxx::lang::jint visible = 0;
        for (::jxx::lang::jint i = 0; i < parent->getComponentCount(); ++i)
        {
            auto component = parent->getComponent(i);
            if (component && component->isVisible()) { total += component->getWidth(); ++visible; }
        }
        if (visible > 1) total += (visible - 1) * hgap_;
        ::jxx::lang::jint x = hgap_;
        if (align_ == CENTER) x = (parent->getWidth() - total) / 2;
        else if (align_ == RIGHT || align_ == TRAILING) x = parent->getWidth() - total - hgap_;
        for (::jxx::lang::jint i = 0; i < parent->getComponentCount(); ++i)
        {
            auto component = parent->getComponent(i);
            if (!component || !component->isVisible()) continue;
            component->setLocation(x, vgap_);
            x += component->getWidth() + hgap_;
        }
    }
}
