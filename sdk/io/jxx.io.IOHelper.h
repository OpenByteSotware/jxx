#pragma once
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
namespace jxx::lang { class String; }
namespace jxx::io {
class IOHelper final {
public:
    static void checkBounds(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length);
private:
    IOHelper() = delete;
};
} // namespace jxx::io
