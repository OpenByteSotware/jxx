#include "util/zip/jxx.util.zip.DataFormatException.h"

namespace jxx::util::zip {

DataFormatException::DataFormatException()
    : Super("DataFormatException") {
}

DataFormatException::DataFormatException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

DataFormatException::DataFormatException(const char* message)
    : Super(message == nullptr ? "DataFormatException" : message) {
}

DataFormatException::DataFormatException(const std::string& message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object> DataFormatException::cloneImpl() const {
    return ::jxx::NEW<DataFormatException>(*this);
}

const char* DataFormatException::typeName() const noexcept {
    return "jxx.util.zip.DataFormatException";
}

} // namespace jxx::util::zip
