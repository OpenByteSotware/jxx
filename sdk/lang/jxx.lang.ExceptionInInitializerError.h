#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang { class ExceptionInInitializerError : public LinkageError
{
	using LinkageError::LinkageError;

protected:
	JXX_OBJECT_CLONE(ExceptionInInitializerError) 
		const char* typeName() const noexcept override { return "ExceptionInInitializerError"; } }; }
