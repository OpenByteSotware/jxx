#include "security/jxx.security.BasicPermission.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

namespace jxx::security {

BasicPermission::BasicPermission(const ::jxx::Ptr<::jxx::lang::String>& name)
    : Super(name) {
    if (name->length() == 0) throw ::jxx::lang::IllegalArgumentException();
    const auto value = name->utf8();
    wildcard_ = value == "*" || (value.size() >= 2 && value.compare(value.size() - 2, 2, ".*") == 0);
    path_ = wildcard_ ? ::jxx::NEW<::jxx::lang::String>(value == "*" ? "" : value.substr(0, value.size() - 1)) : name;
}
BasicPermission::BasicPermission(const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& actions) : BasicPermission(name) { (void)actions; }
::jxx::lang::jbool BasicPermission::implies(const ::jxx::Ptr<Permission>& permission) const {
    const auto other = ::jxx::CAST<BasicPermission>(permission);
    if (other == nullptr || typeid(*this) != typeid(*other)) return false;
    if (wildcard_) return other->name_->utf8().compare(0, path_->utf8().size(), path_->utf8()) == 0;
    return name_->equals(other->name_);
}
::jxx::lang::jbool BasicPermission::equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const {
    const auto other = ::jxx::CAST<BasicPermission>(object);
    return other != nullptr && typeid(*this) == typeid(*other) && name_->equals(other->name_);
}
::jxx::lang::jint BasicPermission::hashCode() const { return name_->hashCode(); }
::jxx::Ptr<::jxx::lang::String> BasicPermission::getActions() const {
    static const auto empty = ::jxx::NEW<::jxx::lang::String>("");
    return empty;
}


void BasicPermission::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    Permission::writeObject(out);
}

void BasicPermission::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    Permission::readObject(in);
    const auto value = name_->utf8();
    if (value.empty()) throw ::jxx::lang::IllegalArgumentException();
    wildcard_ = value == "*" ||
        (value.size() >= 2U && value.compare(value.size() - 2U, 2U, ".*") == 0);
    path_ = wildcard_
        ? ::jxx::NEW<::jxx::lang::String>(
              value == "*" ? "" : value.substr(0U, value.size() - 1U))
        : name_;
}

void BasicPermission::readObjectNoData() {
    Permission::readObjectNoData();
}

} // namespace jxx::security
