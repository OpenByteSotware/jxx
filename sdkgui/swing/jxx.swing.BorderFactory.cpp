#include "swing/jxx.swing.BorderFactory.h"

#include "swing/border/jxx.swing.border.Border.h"
#include "swing/border/jxx.swing.border.CompoundBorder.h"
#include "swing/border/jxx.swing.border.EmptyBorder.h"
#include "swing/border/jxx.swing.border.LineBorder.h"

namespace jxx::swing
{
    ::jxx::Ptr<::jxx::swing::border::Border>
    BorderFactory::createEmptyBorder()
    {
        return ::jxx::NEW<::jxx::swing::border::EmptyBorder>(0, 0, 0, 0);
    }

    ::jxx::Ptr<::jxx::swing::border::Border>
    BorderFactory::createEmptyBorder(
        ::jxx::lang::jint top,
        ::jxx::lang::jint left,
        ::jxx::lang::jint bottom,
        ::jxx::lang::jint right)
    {
        return ::jxx::NEW<::jxx::swing::border::EmptyBorder>(
            top, left, bottom, right);
    }

    ::jxx::Ptr<::jxx::swing::border::Border>
    BorderFactory::createLineBorder(
        const ::jxx::Ptr<::jxx::awt::Color>& color)
    {
        return ::jxx::NEW<::jxx::swing::border::LineBorder>(color);
    }

    ::jxx::Ptr<::jxx::swing::border::Border>
    BorderFactory::createLineBorder(
        const ::jxx::Ptr<::jxx::awt::Color>& color,
        ::jxx::lang::jint thickness)
    {
        return ::jxx::NEW<::jxx::swing::border::LineBorder>(
            color, thickness);
    }

    ::jxx::Ptr<::jxx::swing::border::Border>
    BorderFactory::createLineBorder(
        const ::jxx::Ptr<::jxx::awt::Color>& color,
        ::jxx::lang::jint thickness,
        ::jxx::lang::jbool roundedCorners)
    {
        return ::jxx::NEW<::jxx::swing::border::LineBorder>(
            color, thickness, roundedCorners);
    }

    ::jxx::Ptr<::jxx::swing::border::Border>
    BorderFactory::createCompoundBorder()
    {
        return ::jxx::NEW<::jxx::swing::border::CompoundBorder>();
    }

    ::jxx::Ptr<::jxx::swing::border::Border>
    BorderFactory::createCompoundBorder(
        const ::jxx::Ptr<::jxx::swing::border::Border>& outside,
        const ::jxx::Ptr<::jxx::swing::border::Border>& inside)
    {
        return ::jxx::NEW<::jxx::swing::border::CompoundBorder>(
            outside, inside);
    }
}
