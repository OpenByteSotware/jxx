#pragma once
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::util::concurrent {
class CancellationException : public jxx::lang::IllegalStateException { public: using jxx::lang::IllegalStateException::IllegalStateException; protected: JXX_OBJECT_CLONE(CancellationException) };
}
