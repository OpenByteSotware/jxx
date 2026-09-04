#include "util/jxx.util.IllegalFormatFlagsException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::IllegalFormatFlagsException::Class()
{
    return JxxClassInfoMarker::Class();
}
