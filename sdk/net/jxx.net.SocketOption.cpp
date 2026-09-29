#include "net/jxx.net.SocketOption.h"

#include <utility>

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::net {

BasicSocketOption::BasicSocketOption(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    TypeResolver typeResolver)
    : name_(name), typeResolver_(std::move(typeResolver)) {
    if (name_ == nullptr || !typeResolver_)
        throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::String> BasicSocketOption::name() const {
    return name_;
}

::jxx::Ptr<::jxx::lang::ClassAny> BasicSocketOption::type() const {
    const auto resolved = typeResolver_();
    if (resolved == nullptr)
        throw ::jxx::lang::NullPointerException();
    return resolved;
}

::jxx::Ptr<::jxx::lang::String> BasicSocketOption::toString() const {
    return name_;
}

} // namespace jxx::net
