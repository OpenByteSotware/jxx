#pragma once
#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::awt::geom { class IllegalPathStateException : public ::jxx::lang::ClassBase<IllegalPathStateException,::jxx::lang::RuntimeException>{public: using JxxSuper=::jxx::lang::RuntimeException;using Super=::jxx::lang::ClassBase<IllegalPathStateException,JxxSuper>;IllegalPathStateException()=default;explicit IllegalPathStateException(const ::jxx::Ptr<::jxx::lang::String>& message):Super(message){} }; }
