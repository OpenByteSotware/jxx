#include "io/jxx.io.ObjectInputStream.h"

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

ObjectInputStream::ObjectInputStream(
    const ::jxx::Ptr<InputStream>& input)
    : Super(input) {
}

ObjectInputStream::ObjectInputStream()
    : Super(::jxx::Ptr<InputStream>{}) {
}

ObjectInputStream::~ObjectInputStream() = default;

::jxx::Ptr<::jxx::lang::Object>
ObjectInputStream::readObject() {
    throw IOException(
        ::jxx::NEW<::jxx::lang::String>(
            "Object graph input is not implemented"));
}

::jxx::Ptr<::jxx::lang::Object>
ObjectInputStream::readUnshared() {
    return readObject();
}

} // namespace jxx::io
