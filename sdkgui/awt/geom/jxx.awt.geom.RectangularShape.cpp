#include "awt/geom/jxx.awt.geom.RectangularShape.h"

#include <algorithm>
#include <cmath>

#include "awt/jxx.awt.Rectangle.h"
#include "awt/geom/jxx.awt.geom.Dimension2D.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "awt/geom/jxx.awt.geom.Rectangle2D.h"

namespace jxx::awt::geom
{
    ::jxx::lang::jdouble RectangularShape::getMinX() const { return getX(); }
    ::jxx::lang::jdouble RectangularShape::getMinY() const { return getY(); }
    ::jxx::lang::jdouble RectangularShape::getMaxX() const { return getX() + getWidth(); }
    ::jxx::lang::jdouble RectangularShape::getMaxY() const { return getY() + getHeight(); }
    ::jxx::lang::jdouble RectangularShape::getCenterX() const { return getX() + getWidth() / 2.0; }
    ::jxx::lang::jdouble RectangularShape::getCenterY() const { return getY() + getHeight() / 2.0; }
    ::jxx::Ptr<Rectangle2D> RectangularShape::getFrame() const
    { return ::jxx::NEW<Rectangle2D::Double>(getX(), getY(), getWidth(), getHeight()); }
    void RectangularShape::setFrame(const ::jxx::Ptr<Point2D>& p,
        const ::jxx::Ptr<Dimension2D>& d)
    { if (p != nullptr && d != nullptr) setFrame(p->getX(), p->getY(), d->getWidth(), d->getHeight()); }
    void RectangularShape::setFrameFromDiagonal(::jxx::lang::jdouble x1,
        ::jxx::lang::jdouble y1, ::jxx::lang::jdouble x2,
        ::jxx::lang::jdouble y2)
    { setFrame(std::min(x1,x2), std::min(y1,y2), std::abs(x2-x1), std::abs(y2-y1)); }
    void RectangularShape::setFrameFromCenter(::jxx::lang::jdouble cx,
        ::jxx::lang::jdouble cy, ::jxx::lang::jdouble x,
        ::jxx::lang::jdouble y)
    { const auto dx=std::abs(x-cx); const auto dy=std::abs(y-cy); setFrame(cx-dx,cy-dy,dx*2.0,dy*2.0); }
    ::jxx::Ptr<::jxx::awt::Rectangle> RectangularShape::getBounds() const
    { return ::jxx::NEW<::jxx::awt::Rectangle>(static_cast<::jxx::lang::jint>(std::floor(getX())),static_cast<::jxx::lang::jint>(std::floor(getY())),static_cast<::jxx::lang::jint>(std::ceil(getWidth())),static_cast<::jxx::lang::jint>(std::ceil(getHeight()))); }
    ::jxx::Ptr<PathIterator> RectangularShape::getPathIterator(
        const ::jxx::Ptr<AffineTransform>& transform,
        ::jxx::lang::jdouble) const
    { return getPathIterator(transform); }
}
