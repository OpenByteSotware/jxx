#include "security/jxx.security.Permission.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.InvalidObjectException.h"

namespace jxx::security {

Permission::Permission(const ::jxx::Ptr<::jxx::lang::String>& name)
    : Super()
    , name_(name) {
    if (name_ == nullptr) throw ::jxx::lang::NullPointerException();
}
void Permission::checkGuard(const ::jxx::Ptr<::jxx::lang::Object>& object) { (void)object; }
::jxx::Ptr<::jxx::lang::String> Permission::getName() const { return name_; }
::jxx::Ptr<::jxx::lang::String> Permission::toString() const {
    return ::jxx::NEW<::jxx::lang::String>("(" + name_->utf8() + " " + getActions()->utf8() + ")");
}
void Permission::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw ::jxx::lang::NullPointerException();
    out->writeObject(::jxx::CAST<::jxx::lang::Object>(name_));
}
void Permission::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw ::jxx::lang::NullPointerException();
    name_ = ::jxx::CAST<::jxx::lang::String>(in->readObject());
    if (name_ == nullptr) {
        throw ::jxx::io::InvalidObjectException(
            ::jxx::NEW<::jxx::lang::String>("Permission name is null"));
    }
}
void Permission::readObjectNoData() {
    throw ::jxx::io::InvalidObjectException(
        ::jxx::NEW<::jxx::lang::String>("Permission has no serialized data"));
}

} // namespace jxx::security
