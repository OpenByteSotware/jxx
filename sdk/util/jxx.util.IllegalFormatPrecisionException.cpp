#include "util/jxx.util.IllegalFormatPrecisionException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::IllegalFormatPrecisionException::Class()
{
    return JxxClassInfoMarker::Class();
}
