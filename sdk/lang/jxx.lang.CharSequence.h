#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::lang
{
	class CharSequence
        : public jxx::lang::InterfaceBase<CharSequence>
	{
	public:
		virtual ~CharSequence() = default;
		virtual jint length() const = 0;
		virtual jchar charAt(jint index) const = 0;
		virtual jxx::Ptr<CharSequence> subSequence(jint start, jint end) const = 0;
	};
}
