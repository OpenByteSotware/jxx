#include "text/jxx.text.ClassInitialization.h"

#include <mutex>

#include "io/jxx.io.SerializationClassRegistry.h"
#include "text/jxx.text.DateFormat.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.DecimalFormat.h"
#include "text/jxx.text.Format.h"
#include "text/jxx.text.MessageFormat.h"
#include "text/jxx.text.NumberFormat.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.ParsePosition.h"
#include "text/jxx.text.SimpleDateFormat.h"

namespace jxx::text {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DateFormat::JxxClassInfoMarker::Class();
        (void)DateFormatSymbols::JxxClassInfoMarker::Class();
        ::jxx::io::serialization::registerSerializableClass<DateFormatSymbols>(1);
        (void)DecimalFormat::JxxClassInfoMarker::Class();
        (void)Format::JxxClassInfoMarker::Class();
        (void)MessageFormat::JxxClassInfoMarker::Class();
        (void)NumberFormat::JxxClassInfoMarker::Class();
        (void)ParseException::JxxClassInfoMarker::Class();
        (void)ParsePosition::JxxClassInfoMarker::Class();
        (void)SimpleDateFormat::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::text
