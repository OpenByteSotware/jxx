#include "util/jxx.util.MissingFormatWidthException.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util {

::jxx::Ptr<::jxx::lang::ClassAny> MissingFormatWidthException::Class() {
    return JxxClassInfoMarker::Class();
}

MissingFormatWidthException::MissingFormatWidthException(
    const ::jxx::Ptr<::jxx::lang::String>& formatSpecifier)
    : IllegalFormatException(formatSpecifier)
    , formatSpecifier_(formatSpecifier) {
    if (formatSpecifier_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String>
MissingFormatWidthException::getFormatSpecifier() const {
    return formatSpecifier_;
}

} // namespace jxx::util
