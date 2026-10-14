#include "lang/jxx.lang.StringCaseInsensitiveComparator.h"

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::lang {

jxx::Ptr<ClassAny> StringCaseInsensitiveComparator::Class() {
    return JxxClassInfoMarker::Class();
}

::jxx::lang::jint StringCaseInsensitiveComparator::compare(
    const jxx::Ptr<String>& first,
    const jxx::Ptr<String>& second) const {
    if (first == nullptr || second == nullptr) {
        throw NullPointerException();
    }
    return first->compareToIgnoreCase(second);
}

} // namespace jxx::lang
