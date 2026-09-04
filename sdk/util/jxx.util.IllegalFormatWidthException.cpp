#include "util/jxx.util.IllegalFormatWidthException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::IllegalFormatWidthException::Class()
{
    return JxxClassInfoMarker::Class();
}
