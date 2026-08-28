#pragma once
#include "lang/jxx_types.h"
namespace jxx::lang
{
	class CharSequence
	{
	public:
		virtual ~CharSequence() = default;
		virtual jint length() const = 0;
		virtual jchar charAt(jint index) const = 0;
		virtual jxx::Ptr<CharSequence> subSequence(jint start, jint end) const = 0;
	};
}
