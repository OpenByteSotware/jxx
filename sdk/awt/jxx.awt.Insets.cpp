#include "awt/jxx.awt.Insets.h"
#include <cstdint>
#include <sstream>
namespace jxx::awt
{
	Insets::Insets(::jxx::lang::jint t, ::jxx::lang::jint l, ::jxx::lang::jint b, ::jxx::lang::jint r) :top(t), left(l), bottom(b), right(r)
	{
	} void Insets::set(::jxx::lang::jint t, ::jxx::lang::jint l, ::jxx::lang::jint b, ::jxx::lang::jint r)
	{
		top = t; left = l; bottom = b; right = r;
	} ::jxx::lang::jbool Insets::equals(const ::jxx::Ptr<::jxx::lang::Object>& o) const
	{
		auto i = ::jxx::CAST<Insets>(o); return i && i->top == top && i->left == left && i->bottom == bottom && i->right == right;
	} ::jxx::lang::jint Insets::hashCode() const
	{
		std::uint32_t h = static_cast<std::uint32_t>(top); h = h * 31u + static_cast<std::uint32_t>(left); h = h * 31u + static_cast<std::uint32_t>(bottom); h = h * 31u + static_cast<std::uint32_t>(right); return static_cast<::jxx::lang::jint>(h);
	} ::jxx::Ptr<::jxx::lang::String> Insets::toString() const
	{
		std::ostringstream s; s << "jxx.awt.Insets[top=" << top << ",left=" << left << ",bottom=" << bottom << ",right=" << right << "]"; return ::jxx::NEW<::jxx::lang::String>(s.str());
	}
}
