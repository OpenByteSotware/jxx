#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace com::google::gson {

::jxx::Ptr<::jxx::lang::ClassAny> JsonSyntaxException::Class() {
    return JxxClassInfoMarker::Class();
}

JsonSyntaxException::JsonSyntaxException()
    : JxxSuper() {}

JsonSyntaxException::JsonSyntaxException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {}

::jxx::Ptr<::jxx::lang::Object> JsonSyntaxException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<JsonSyntaxException>(*this));
}

const char* JsonSyntaxException::typeName() const noexcept {
    return "com.google.gson.JsonSyntaxException";
}

} // namespace com::google::gson
