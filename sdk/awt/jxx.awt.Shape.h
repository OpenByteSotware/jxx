#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class Rectangle;
}

namespace jxx::awt::geom
{
    class AffineTransform;
    class PathIterator;
    class Point2D;
    class Rectangle2D;
}

namespace jxx::awt
{
    class Shape : public ::jxx::lang::InterfaceBase<Shape>
    {
    public:
        ~Shape() override = default;

        virtual ::jxx::Ptr<Rectangle> getBounds() const = 0;
        virtual ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>
            getBounds2D() const = 0;
        virtual ::jxx::lang::jbool contains(
            ::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y) const = 0;
        virtual ::jxx::lang::jbool contains(
            const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point) const = 0;
        virtual ::jxx::lang::jbool intersects(
            ::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y,
            ::jxx::lang::jdouble width,
            ::jxx::lang::jdouble height) const = 0;
        virtual ::jxx::lang::jbool intersects(
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& rectangle) const = 0;
        virtual ::jxx::lang::jbool contains(
            ::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y,
            ::jxx::lang::jdouble width,
            ::jxx::lang::jdouble height) const = 0;
        virtual ::jxx::lang::jbool contains(
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& rectangle) const = 0;
        virtual ::jxx::Ptr<::jxx::awt::geom::PathIterator>
            getPathIterator(
                const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform) const = 0;
        virtual ::jxx::Ptr<::jxx::awt::geom::PathIterator>
            getPathIterator(
                const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform,
                ::jxx::lang::jdouble flatness) const = 0;
    };
}
