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

	::jxx::lang::jdouble Point::getX() const { return static_cast<::jxx::lang::jdouble>(x); }
	::jxx::lang::jdouble Point::getY() const { return static_cast<::jxx::lang::jdouble>(y); }
	void Point::setLocation(::jxx::lang::jdouble xValue, ::jxx::lang::jdouble yValue)
	{
		x = static_cast<::jxx::lang::jint>(xValue);
		y = static_cast<::jxx::lang::jint>(yValue);
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

	void Point::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output)
	{
		output->writeInt(x); output->writeInt(y);
	}
	void Point::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input)
	{
		x = input->readInt(); y = input->readInt();
	}
	void Point::readObjectNoData() { x = 0; y = 0; }
}
