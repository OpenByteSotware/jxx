#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang
{
	class BootstrapMethodError :
		public LinkageError
	{
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<BootstrapMethodError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    BootstrapMethodError() = default;

    BootstrapMethodError(const BootstrapMethodError&) = default;
    BootstrapMethodError(BootstrapMethodError&&) noexcept = default;
    BootstrapMethodError& operator=(const BootstrapMethodError&) = default;
    BootstrapMethodError& operator=(BootstrapMethodError&&) noexcept = default;
    ~BootstrapMethodError() override = default;

	public:
		using LinkageError::LinkageError;
	protected:
		JXX_OBJECT_CLONE(BootstrapMethodError)
			const char* typeName() const noexcept override
		{
			return "BootstrapMethodError";
		}
	};
}
