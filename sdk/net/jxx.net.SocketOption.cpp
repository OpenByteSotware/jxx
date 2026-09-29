#include "net/jxx.net.SocketOption.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::net {

BasicSocketOption::BasicSocketOption(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type)
    : name_(name), type_(type) {
    if (name_ == nullptr || type_ == nullptr)
        throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::String> BasicSocketOption::name() const {
    return name_;
}

::jxx::Ptr<::jxx::lang::ClassAny> BasicSocketOption::type() const {
    return type_;
}

::jxx::Ptr<::jxx::lang::String> BasicSocketOption::toString() const {
    return name_;
}

} // namespace jxx::net
