#include "util/jxx.util.UnknownFormatFlagsException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::UnknownFormatFlagsException::Class()
{
    return JxxClassInfoMarker::Class();
}
