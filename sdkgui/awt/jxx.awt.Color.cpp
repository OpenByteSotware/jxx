#include "awt/jxx.awt.Color.h"
#include <algorithm>
#include <cstdint>
#include <sstream>
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::awt
{
	namespace
	{
		void validate(::jxx::lang::jint value)
		{
			if (value < 0 || value > 255) throw ::jxx::lang::IllegalArgumentException("Color parameter outside expected range");
		}
		::jxx::lang::jint pack(::jxx::lang::jint r, ::jxx::lang::jint g, ::jxx::lang::jint b, ::jxx::lang::jint a)
		{
			validate(r); validate(g); validate(b); validate(a); return static_cast<::jxx::lang::jint>((static_cast<std::uint32_t>(a) << 24) | (static_cast<std::uint32_t>(r) << 16) | (static_cast<std::uint32_t>(g) << 8) | static_cast<std::uint32_t>(b));
		}
		::jxx::Ptr<Color> make(::jxx::lang::jint r, ::jxx::lang::jint g, ::jxx::lang::jint b)
		{
			return ::jxx::NEW<Color>(r, g, b);
		}
	}
	Color::Color(::jxx::lang::jint rgb) :value_(static_cast<::jxx::lang::jint>(0xff000000u | static_cast<std::uint32_t>(rgb)))
	{
	}
	Color::Color(::jxx::lang::jint r, ::jxx::lang::jint g, ::jxx::lang::jint b) :value_(pack(r, g, b, 255))
	{
	}
	Color::Color(::jxx::lang::jint r, ::jxx::lang::jint g, ::jxx::lang::jint b, ::jxx::lang::jint a) :value_(pack(r, g, b, a))
	{
	}
	::jxx::lang::jint Color::getRed() const
	{
		return (static_cast<std::uint32_t>(value_) >> 16) & 0xffu;
	} ::jxx::lang::jint Color::getGreen() const
	{
		return (static_cast<std::uint32_t>(value_) >> 8) & 0xffu;
	} ::jxx::lang::jint Color::getBlue() const
	{
		return static_cast<std::uint32_t>(value_) & 0xffu;
	} ::jxx::lang::jint Color::getAlpha() const
	{
		return (static_cast<std::uint32_t>(value_) >> 24) & 0xffu;
	} ::jxx::lang::jint Color::getRGB() const
	{
		return value_;
	}
	::jxx::Ptr<Color> Color::brighter() const
	{
		constexpr int f = 3; int r = getRed(), g = getGreen(), b = getBlue(); if (r == 0 && g == 0 && b == 0)return ::jxx::NEW<Color>(3, 3, 3, getAlpha()); return ::jxx::NEW<Color>(std::min(255, r * 10 / 7), std::min(255, g * 10 / 7), std::min(255, b * 10 / 7), getAlpha());
	}
	::jxx::Ptr<Color> Color::darker() const
	{
		return ::jxx::NEW<Color>(getRed() * 7 / 10, getGreen() * 7 / 10, getBlue() * 7 / 10, getAlpha());
	}
	::jxx::lang::jbool Color::equals(const ::jxx::Ptr<::jxx::lang::Object>& o) const
	{
		auto c = ::jxx::CAST<Color>(o); return c && c->value_ == value_;
	} ::jxx::lang::jint Color::hashCode() const
	{
		return value_;
	}
	::jxx::Ptr<::jxx::lang::String> Color::toString() const
	{
		std::ostringstream s; s << "jxx.awt.Color[r=" << getRed() << ",g=" << getGreen() << ",b=" << getBlue() << "]"; return ::jxx::NEW<::jxx::lang::String>(s.str());
	}
	::jxx::Ptr<Color> Color::white = make(255, 255, 255); ::jxx::Ptr<Color> Color::WHITE = Color::white; ::jxx::Ptr<Color> Color::lightGray = make(192, 192, 192); ::jxx::Ptr<Color> Color::LIGHT_GRAY = Color::lightGray; ::jxx::Ptr<Color> Color::gray = make(128, 128, 128); ::jxx::Ptr<Color> Color::GRAY = Color::gray; ::jxx::Ptr<Color> Color::darkGray = make(64, 64, 64); ::jxx::Ptr<Color> Color::DARK_GRAY = Color::darkGray; ::jxx::Ptr<Color> Color::black = make(0, 0, 0); ::jxx::Ptr<Color> Color::BLACK = Color::black; ::jxx::Ptr<Color> Color::red = make(255, 0, 0); ::jxx::Ptr<Color> Color::RED = Color::red; ::jxx::Ptr<Color> Color::pink = make(255, 175, 175); ::jxx::Ptr<Color> Color::PINK = Color::pink; ::jxx::Ptr<Color> Color::orange = make(255, 200, 0); ::jxx::Ptr<Color> Color::ORANGE = Color::orange; ::jxx::Ptr<Color> Color::yellow = make(255, 255, 0); ::jxx::Ptr<Color> Color::YELLOW = Color::yellow; ::jxx::Ptr<Color> Color::green = make(0, 255, 0); ::jxx::Ptr<Color> Color::GREEN = Color::green; ::jxx::Ptr<Color> Color::magenta = make(255, 0, 255); ::jxx::Ptr<Color> Color::MAGENTA = Color::magenta; ::jxx::Ptr<Color> Color::cyan = make(0, 255, 255); ::jxx::Ptr<Color> Color::CYAN = Color::cyan; ::jxx::Ptr<Color> Color::blue = make(0, 0, 255); ::jxx::Ptr<Color> Color::BLUE = Color::blue;
}
