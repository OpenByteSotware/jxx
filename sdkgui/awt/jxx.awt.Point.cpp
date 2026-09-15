#include "awt/jxx.awt.Point.h"
#include <cstdint>
#include <sstream>
namespace jxx::awt
{
	Point::Point(::jxx::lang::jint xValue, ::jxx::lang::jint yValue) : x(xValue), y(yValue)
	{
	}
	Point::Point(const ::jxx::Ptr<Point>& point)
	{
		if (point) {
			x = point->x; y = point->y;
		}
	}
	::jxx::Ptr<Point> Point::getLocation() const
	{
		return ::jxx::NEW<Point>(x, y);
	}
	void Point::setLocation(const ::jxx::Ptr<Point>& point)
	{
		if (point) setLocation(point->x, point->y);
	}
	void Point::setLocation(::jxx::lang::jint xValue, ::jxx::lang::jint yValue)
	{
		x = xValue; y = yValue;
	}
	void Point::move(::jxx::lang::jint xValue, ::jxx::lang::jint yValue)
	{
		setLocation(xValue, yValue);
	}
	void Point::translate(::jxx::lang::jint dx, ::jxx::lang::jint dy)
	{
		x += dx; y += dy;
	}
	::jxx::lang::jbool Point::equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const
	{
		auto p = ::jxx::CAST<Point>(object); return p && p->x == x && p->y == y;
	}
	::jxx::lang::jint Point::hashCode() const
	{
		const auto ux = static_cast<std::uint32_t>(x); const auto uy = static_cast<std::uint32_t>(y); return static_cast<::jxx::lang::jint>(ux ^ (uy * 31u));
	}
	::jxx::Ptr<::jxx::lang::String> Point::toString() const
	{
		std::ostringstream s; s << "jxx.awt.Point[x=" << x << ",y=" << y << "]"; return ::jxx::NEW<::jxx::lang::String>(s.str());
	}
}
