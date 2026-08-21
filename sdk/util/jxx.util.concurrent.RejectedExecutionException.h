#pragma once
#include "lang/jxx.lang.RuntimeException.h"
namespace jxx::util::concurrent {
class RejectedExecutionException : public jxx::lang::RuntimeException { public: using jxx::lang::RuntimeException::RuntimeException; protected: JXX_OBJECT_CLONE(RejectedExecutionException) };
}
