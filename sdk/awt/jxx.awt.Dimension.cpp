#include "awt/jxx.awt.Dimension.h"
#include <cstdint>
#include <sstream>
namespace jxx::awt
{
	Dimension::Dimension(::jxx::lang::jint w, ::jxx::lang::jint h) :width(w), height(h)
	{
	}
	Dimension::Dimension(const ::jxx::Ptr<Dimension>& d)
	{
		if (d) {
			width = d->width; height = d->height;
		}
	}
	::jxx::Ptr<Dimension> Dimension::getSize() const
	{
		return ::jxx::NEW<Dimension>(width, height);
	} void Dimension::setSize(const ::jxx::Ptr<Dimension>& d)
	{
		if (d)setSize(d->width, d->height);
	} void Dimension::setSize(::jxx::lang::jint w, ::jxx::lang::jint h)
	{
		width = w; height = h;
	}
	::jxx::lang::jbool Dimension::equals(const ::jxx::Ptr<::jxx::lang::Object>& o) const
	{
		auto d = ::jxx::CAST<Dimension>(o); return d && d->width == width && d->height == height;
	}
	::jxx::lang::jint Dimension::hashCode() const
	{
		return static_cast<::jxx::lang::jint>(static_cast<std::uint32_t>(width) * 31u + static_cast<std::uint32_t>(height));
	}
	::jxx::Ptr<::jxx::lang::String> Dimension::toString() const
	{
		std::ostringstream s; s << "jxx.awt.Dimension[width=" << width << ",height=" << height << "]"; return ::jxx::NEW<::jxx::lang::String>(s.str());
	}
}
