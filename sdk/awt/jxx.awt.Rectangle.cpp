#include "awt/jxx.awt.Rectangle.h"
#include <algorithm>
#include <cstdint>
#include <sstream>
namespace jxx::awt
{
	Rectangle::Rectangle(::jxx::lang::jint xv, ::jxx::lang::jint yv, ::jxx::lang::jint w, ::jxx::lang::jint h) :x(xv), y(yv), width(w), height(h)
	{
	} Rectangle::Rectangle(const ::jxx::Ptr<Dimension>& d)
	{
		if (d) {
			width = d->width; height = d->height;
		}
	} Rectangle::Rectangle(const ::jxx::Ptr<Point>& p, const ::jxx::Ptr<Dimension>& d)
	{
		if (p) {
			x = p->x; y = p->y;
		}if (d) {
			width = d->width; height = d->height;
		}
	} ::jxx::Ptr<Point> Rectangle::getLocation() const
	{
		return ::jxx::NEW<Point>(x, y);
	} ::jxx::Ptr<Dimension> Rectangle::getSize() const
	{
		return ::jxx::NEW<Dimension>(width, height);
	} void Rectangle::setBounds(::jxx::lang::jint xv, ::jxx::lang::jint yv, ::jxx::lang::jint w, ::jxx::lang::jint h)
	{
		x = xv; y = yv; width = w; height = h;
	} ::jxx::lang::jbool Rectangle::contains(::jxx::lang::jint px, ::jxx::lang::jint py) const
	{
		return width > 0 && height > 0 && px >= x && py >= y && px < x + width && py < y + height;
	} ::jxx::lang::jbool Rectangle::contains(const ::jxx::Ptr<Rectangle>& r) const
	{
        if (r == nullptr || width < 0 || height < 0 ||
            r->width < 0 || r->height < 0) return false;
        const auto left = static_cast<std::int64_t>(x);
        const auto top = static_cast<std::int64_t>(y);
        const auto right = left + static_cast<std::int64_t>(width);
        const auto bottom = top + static_cast<std::int64_t>(height);
        const auto otherLeft = static_cast<std::int64_t>(r->x);
        const auto otherTop = static_cast<std::int64_t>(r->y);
        const auto otherRight = otherLeft + static_cast<std::int64_t>(r->width);
        const auto otherBottom = otherTop + static_cast<std::int64_t>(r->height);
        return otherLeft >= left && otherTop >= top &&
            otherRight <= right && otherBottom <= bottom;
	} ::jxx::lang::jbool Rectangle::intersects(const ::jxx::Ptr<Rectangle>& r) const
	{
		return r && width > 0 && height > 0 && r->width > 0 && r->height > 0 && r->x + r->width > x && r->y + r->height > y && r->x < x + width && r->y < y + height;
	} ::jxx::lang::jbool Rectangle::isEmpty() const
	{
		return width <= 0 || height <= 0;
	} ::jxx::lang::jbool Rectangle::equals(const ::jxx::Ptr<::jxx::lang::Object>& o) const
	{
		auto r = ::jxx::CAST<Rectangle>(o); return r && r->x == x && r->y == y && r->width == width && r->height == height;
	} ::jxx::lang::jint Rectangle::hashCode() const
	{
		std::uint32_t h = static_cast<std::uint32_t>(x); h = h * 31u + static_cast<std::uint32_t>(y); h = h * 31u + static_cast<std::uint32_t>(width); h = h * 31u + static_cast<std::uint32_t>(height); return static_cast<::jxx::lang::jint>(h);
	} ::jxx::Ptr<::jxx::lang::String> Rectangle::toString() const
	{
		std::ostringstream s; s << "jxx.awt.Rectangle[x=" << x << ",y=" << y << ",width=" << width << ",height=" << height << "]"; return ::jxx::NEW<::jxx::lang::String>(s.str());
	}
::jxx::Ptr<Rectangle> Rectangle::intersection(const ::jxx::Ptr<Rectangle>& r) const { if(!r)return ::jxx::NEW<Rectangle>(); const auto nx=std::max(x,r->x),ny=std::max(y,r->y),nr=std::min(x+width,r->x+r->width),nb=std::min(y+height,r->y+r->height); return ::jxx::NEW<Rectangle>(nx,ny,nr-nx,nb-ny);}
}
