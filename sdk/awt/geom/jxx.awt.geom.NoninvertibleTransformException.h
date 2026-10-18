#pragma once
#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.String.h"
namespace jxx::awt::geom { class NoninvertibleTransformException : public ::jxx::lang::ClassBase<NoninvertibleTransformException,::jxx::lang::Exception>{public: using JxxSuper=::jxx::lang::Exception;using Super=::jxx::lang::ClassBase<NoninvertibleTransformException,JxxSuper>;explicit NoninvertibleTransformException(const ::jxx::Ptr<::jxx::lang::String>& message):Super(message){} }; }
