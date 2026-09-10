#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io {
void IOHelper::checkBounds(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length) {
    if (!buffer) throw ::jxx::lang::NullPointerException(::jxx::NEW<::jxx::lang::String>("buffer"));
    if (offset < 0 || length < 0 || offset > static_cast<::jxx::lang::jint>(buffer->length) - length)
        throw ::jxx::lang::IndexOutOfBoundsException(::jxx::NEW<::jxx::lang::String>("offset/length"));
}
} // namespace jxx::io
