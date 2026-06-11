#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class StringPool {
public:
    static jxx::Ptr<jxx::lang::String> intern(const char* text);
};

} // namespace jxx::util
