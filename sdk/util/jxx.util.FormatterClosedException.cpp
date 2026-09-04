#include "util/jxx.util.FormatterClosedException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::FormatterClosedException::Class()
{
    return JxxClassInfoMarker::Class();
}
