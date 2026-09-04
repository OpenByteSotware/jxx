#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang { class ExceptionInInitializerError : public LinkageError
{
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ExceptionInInitializerError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ExceptionInInitializerError() = default;

    ExceptionInInitializerError(const ExceptionInInitializerError&) = default;
    ExceptionInInitializerError(ExceptionInInitializerError&&) noexcept = default;
    ExceptionInInitializerError& operator=(const ExceptionInInitializerError&) = default;
    ExceptionInInitializerError& operator=(ExceptionInInitializerError&&) noexcept = default;
    ~ExceptionInInitializerError() override = default;

	using LinkageError::LinkageError;

protected:
	JXX_OBJECT_CLONE(ExceptionInInitializerError) 
		const char* typeName() const noexcept override { return "ExceptionInInitializerError"; } }; }
