#include "ext/xml/validation/jxx.ext.xml.validation.ClassInitialization.h"

#include <mutex>

namespace jxx::ext::xml::validation {

void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {});
}

} // namespace jxx::ext::xml::validation
