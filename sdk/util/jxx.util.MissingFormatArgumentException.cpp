#include "util/jxx.util.MissingFormatArgumentException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::MissingFormatArgumentException::Class()
{
    return JxxClassInfoMarker::Class();
}
