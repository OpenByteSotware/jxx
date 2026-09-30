#pragma once
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::nio::channels { class ClosedSelectorException:public ::jxx::lang::IllegalStateException{public:using IllegalStateException::IllegalStateException;}; }
