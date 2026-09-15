#include "lang/jxx.lang.Package.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {

Package::Package(const jxx::Ptr<String>& name)
    : name_(name) {
    if (name_ == nullptr) {
        throw NullPointerException("name");
    }
}

jxx::Ptr<String> Package::getName() const {
    return name_;
}

jxx::Ptr<String> Package::toString() const {
    return jxx::NEW<String>(
        std::string("package ") + name_->utf8());
}

} // namespace jxx::lang
