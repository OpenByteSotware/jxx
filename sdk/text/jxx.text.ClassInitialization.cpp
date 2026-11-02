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
        (void)DateFormat::Class();
        (void)DateFormatSymbols::Class();
        ::jxx::io::serialization::registerSerializableClass<DateFormatSymbols>(
            static_cast<::jxx::lang::jlong>(-5987973545549424702LL));
        (void)DecimalFormat::Class();
        (void)Format::Class();
        (void)MessageFormat::Class();
        (void)NumberFormat::Class();
        (void)ParseException::Class();
        (void)ParsePosition::Class();
        (void)SimpleDateFormat::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::text
