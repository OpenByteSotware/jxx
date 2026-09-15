#pragma once
#include "lang/jxx_types.h"
namespace jxx::gui::internal { class GuiRuntime final { public: static ::jxx::lang::jint run(); static void exit(); static ::jxx::lang::jbool isEventDispatchThread(); private: GuiRuntime()=delete; }; }
