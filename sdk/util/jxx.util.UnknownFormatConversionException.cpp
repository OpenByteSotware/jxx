#include "util/jxx.util.UnknownFormatConversionException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::UnknownFormatConversionException::Class()
{
    return JxxClassInfoMarker::Class();
}
