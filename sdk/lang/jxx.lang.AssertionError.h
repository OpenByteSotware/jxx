#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Error.h"
namespace jxx::lang
{
	class AssertionError : public Error
	{
public:
    using JxxSuper = Error;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<AssertionError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    AssertionError() = default;

    AssertionError(const AssertionError&) = default;
    AssertionError(AssertionError&&) noexcept = default;
    AssertionError& operator=(const AssertionError&) = default;
    AssertionError& operator=(AssertionError&&) noexcept = default;
    ~AssertionError() override = default;

	public:
		using Error::Error;

	protected:
		JXX_OBJECT_CLONE(AssertionError)
			const char* typeName() const noexcept override
		{
			return "AssertionError";
		}
	};
}
