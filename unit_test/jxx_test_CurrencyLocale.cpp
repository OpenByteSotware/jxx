#include <gtest/gtest.h>
#include <locale>
#include "jxx.h"

using namespace jxx::lang;

static bool hasLocale(const char* name){ try { std::locale loc(name); (void)loc; return true; } catch(...) {return false;} }
