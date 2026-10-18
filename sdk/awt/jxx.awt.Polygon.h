#pragma once

#include <vector>

#include "awt/jxx.awt.Shape.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::io
{
    class ObjectInputStream;
    class ObjectOutputStream;
}

namespace jxx::awt
{
    class Point;
    class Polygon : public ::jxx::lang::ClassBase<Polygon,
        ::jxx::lang::Object, Shape, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Polygon, JxxSuper,
            Shape, ::jxx::io::SerializableI>;

        Polygon();
        Polygon(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>& xpoints,
            const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>& ypoints,
            ::jxx::lang::jint npoints);
        void reset();
        void invalidate();
        void translate(::jxx::lang::jint deltaX, ::jxx::lang::jint deltaY);
        void addPoint(::jxx::lang::jint x, ::jxx::lang::jint y);
        ::jxx::Ptr<Rectangle> getBounds() const override;
        ::jxx::Ptr<::jxx::awt::geom::Rectangle2D> getBounds2D() const override;
        ::jxx::lang::jbool contains(::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y) const override;
        ::jxx::lang::jbool contains(
            const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point) const override;
        ::jxx::lang::jbool intersects(::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y, ::jxx::lang::jdouble width,
            ::jxx::lang::jdouble height) const override;
        ::jxx::lang::jbool intersects(
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& rectangle) const override;
        ::jxx::lang::jbool contains(::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y, ::jxx::lang::jdouble width,
            ::jxx::lang::jdouble height) const override;
        ::jxx::lang::jbool contains(
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& rectangle) const override;
        ::jxx::Ptr<::jxx::awt::geom::PathIterator> getPathIterator(
            const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform) const override;
        ::jxx::Ptr<::jxx::awt::geom::PathIterator> getPathIterator(
            const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform,
            ::jxx::lang::jdouble flatness) const override;
        void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>> xpoints;
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>> ypoints;
        ::jxx::lang::jint npoints = 0;

    private:
        std::vector<::jxx::lang::jint> xs_, ys_;
        void syncArrays();
    };
}
