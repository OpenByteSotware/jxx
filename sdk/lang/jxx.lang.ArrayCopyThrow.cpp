#include "lang/jxx.lang.ArrayCopyThrow.h"

#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {

[[noreturn]] void throwArrayCopyNullPointer() {
    throw ::jxx::lang::NullPointerException();
}

[[noreturn]] void throwArrayCopyIndexOutOfBounds() {
    throw ::jxx::lang::IndexOutOfBoundsException(
        "System.arraycopy: index out of bounds");
}

} // namespace jxx
