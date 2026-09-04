#include "util/jxx.util.InputMismatchException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::InputMismatchException::Class()
{
    return JxxClassInfoMarker::Class();
}
