#include "util/zip/jxx.util.zip.ZipException.h"

namespace jxx::util::zip {

::jxx::Ptr<::jxx::lang::ClassAny>
ZipException::Class() {
    return JxxClassInfoMarker::Class();
}

ZipException::ZipException()
    : Super("ZipException") {
}

ZipException::ZipException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

ZipException::ZipException(
    const char* message)
    : Super(
          message == nullptr
              ? "ZipException"
              : message) {
}

ZipException::ZipException(
    const std::string& message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object>
ZipException::cloneImpl() const {
    return ::jxx::NEW<ZipException>(*this);
}

const char*
ZipException::typeName() const noexcept {
    return "jxx.util.zip.ZipException";
}

} // namespace jxx::util::zip
