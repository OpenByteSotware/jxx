#include "jxx.lang.Package.h"

namespace jxx::lang {

Package::Package(jxx::Ptr<String> name) : name_(name) {}

jxx::Ptr<String> Package::getName() const { return name_; }

jxx::Ptr<String> Package::toString() const {
    // Java: "package <name>"
    std::string s = "package ";
    s += name_ ? name_->utf8() : std::string("null");
    return jxx::NEW<String>(s.c_str());
}

} // namespace jxx::lang