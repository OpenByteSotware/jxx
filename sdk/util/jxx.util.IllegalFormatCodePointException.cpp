#include "util/jxx.util.IllegalFormatCodePointException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::IllegalFormatCodePointException::Class()
{
    return JxxClassInfoMarker::Class();
}
