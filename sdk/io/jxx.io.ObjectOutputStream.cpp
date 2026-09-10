#include "io/jxx.io.ObjectOutputStream.h"

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

ObjectOutputStream::ObjectOutputStream(
    const ::jxx::Ptr<OutputStream>& output)
    : Super(output) {
}

ObjectOutputStream::ObjectOutputStream()
    : Super(::jxx::Ptr<OutputStream>{}) {
}

ObjectOutputStream::~ObjectOutputStream() = default;

void ObjectOutputStream::writeObject(
    const ::jxx::Ptr<::jxx::lang::Object>& object) {

    (void)object;

    throw IOException(
        ::jxx::NEW<::jxx::lang::String>(
            "Object graph output is not implemented"));
}

void ObjectOutputStream::writeUnshared(
    const ::jxx::Ptr<::jxx::lang::Object>& object) {
    writeObject(object);
}

} // namespace jxx::io
