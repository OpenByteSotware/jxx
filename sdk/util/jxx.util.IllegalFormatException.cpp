#include "util/jxx.util.IllegalFormatException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::IllegalFormatException::Class()
{
    return JxxClassInfoMarker::Class();
}
