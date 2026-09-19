#include "swing/jxx.swing.Box.h"

#include "awt/jxx.awt.Dimension.h"
#include "swing/jxx.swing.BoxLayout.h"

namespace jxx::swing
{
    Box::Box(::jxx::lang::jint axis) : Super()
    {
        setLayout(::jxx::NEW<BoxLayout>(
            ::jxx::CAST<::jxx::awt::Container>(thisPtr()), axis));
    }

    ::jxx::Ptr<Box> Box::createHorizontalBox()
    { return ::jxx::NEW<Box>(BoxLayout::X_AXIS); }

    ::jxx::Ptr<Box> Box::createVerticalBox()
    { return ::jxx::NEW<Box>(BoxLayout::Y_AXIS); }

    ::jxx::Ptr<::jxx::awt::Component> Box::createRigidArea(
        const ::jxx::Ptr<::jxx::awt::Dimension>& dimension)
    {
        auto filler = ::jxx::NEW<Box>(BoxLayout::X_AXIS);
        if (dimension) filler->setSize(dimension);
        return filler;
    }

    ::jxx::Ptr<::jxx::awt::Component> Box::createHorizontalStrut(
        ::jxx::lang::jint width)
    { return createRigidArea(::jxx::NEW<::jxx::awt::Dimension>(width, 0)); }

    ::jxx::Ptr<::jxx::awt::Component> Box::createVerticalStrut(
        ::jxx::lang::jint height)
    { return createRigidArea(::jxx::NEW<::jxx::awt::Dimension>(0, height)); }

    ::jxx::Ptr<::jxx::awt::Component> Box::createGlue()
    { return createRigidArea(::jxx::NEW<::jxx::awt::Dimension>(0, 0)); }

    ::jxx::Ptr<::jxx::awt::Component> Box::createHorizontalGlue()
    { return createGlue(); }

    ::jxx::Ptr<::jxx::awt::Component> Box::createVerticalGlue()
    { return createGlue(); }
}
