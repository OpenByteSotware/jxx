#pragma once
#include "lang/jxx.lang.Exception.h"
namespace jxx::util::concurrent {
class ExecutionException : public jxx::lang::Exception { public: using jxx::lang::Exception::Exception; protected: JXX_OBJECT_CLONE(ExecutionException) };
}
