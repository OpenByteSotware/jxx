#include "util/jxx.util.IllegalFormatConversionException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::IllegalFormatConversionException::Class()
{
    return JxxClassInfoMarker::Class();
}
