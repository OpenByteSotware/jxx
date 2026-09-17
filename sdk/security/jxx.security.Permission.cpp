#include "security/jxx.security.Permission.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

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
void Permission::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) { (void)out; }
void Permission::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) { (void)in; }
void Permission::readObjectNoData() {}

} // namespace jxx::security
