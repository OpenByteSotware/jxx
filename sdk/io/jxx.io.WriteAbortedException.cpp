#include <string>
#include "io/jxx.io.WriteAbortedException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

::jxx::Ptr<::jxx::lang::ClassAny> WriteAbortedException::Class() {
    return JxxClassInfoMarker::Class();
}

WriteAbortedException::WriteAbortedException(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    const ::jxx::Ptr<::jxx::lang::Exception>& exception)
    : JxxSuper(message)
    , detail(exception) {
}

::jxx::Ptr<::jxx::lang::Throwable>
WriteAbortedException::getCause() const {
    return detail;
}

::jxx::Ptr<::jxx::lang::String>
WriteAbortedException::getMessage() const {
    const auto message = JxxSuper::getMessage();
    if (detail == nullptr) {
        return message;
    }

    const auto detailText = detail->toString();
    std::string result =
        message == nullptr ? std::string() : message->utf8();
    result += "; ";
    if (detailText != nullptr) {
        result += detailText->utf8();
    }
    return ::jxx::NEW<::jxx::lang::String>(result);
}

::jxx::Ptr<::jxx::lang::Object>
WriteAbortedException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<WriteAbortedException>(*this));
}

const char* WriteAbortedException::typeName() const noexcept {
    return "WriteAbortedException";
}

} // namespace jxx::io
