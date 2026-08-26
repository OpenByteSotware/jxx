#include "jxx.lang.Package.h"

namespace jxx::lang {

Package::Package(const jxx::Ptr<String> name) : name_(name) {}

jxx::Ptr<String> Package::getName() const { return name_; }

} // namespace jxx::lang