#include "text/jxx.text.ClassInitialization.h"

#include <mutex>

#include "text/jxx.text.DateFormat.h"
#include "text/jxx.text.DecimalFormat.h"
#include "text/jxx.text.Format.h"
#include "text/jxx.text.MessageFormat.h"
#include "text/jxx.text.NumberFormat.h"

namespace jxx::text {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DateFormat::JxxClassInfoMarker::Class();
        (void)DecimalFormat::JxxClassInfoMarker::Class();
        (void)Format::JxxClassInfoMarker::Class();
        (void)MessageFormat::JxxClassInfoMarker::Class();
        (void)NumberFormat::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::text
