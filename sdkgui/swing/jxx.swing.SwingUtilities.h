#pragma once
#include "lang/jxx_types.h"
namespace jxx::swing { class SwingUtilities final { public: static ::jxx::lang::jint run(); static ::jxx::lang::jbool isEventDispatchThread(); private: SwingUtilities()=delete; }; }
