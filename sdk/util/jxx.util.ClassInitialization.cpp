#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.ClassInitialization.Detail.h"
#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/function/jxx.util.function.ClassInitialization.h"
#include "util/logging/jxx.util.logging.ClassInitialization.h"
#include "util/regex/jxx.util.regex.ClassInitialization.h"
#include "util/zip/jxx.util.zip.ClassInitialization.h"
#include "util/jar/jxx.util.jar.ClassInitialization.h"
#include "util/stream/jxx.util.stream.ClassInitialization.h"

#include <mutex>

#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.Base64.h"
#include "util/jxx.util.BitSet.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.Collections.h"
#include "util/jxx.util.Currency.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.DoubleSummaryStatistics.h"
#include "util/jxx.util.EventListener.h"
#include "util/jxx.util.EventObject.h"
#include "util/jxx.util.Formattable.h"
#include "util/jxx.util.FormattableFlags.h"
#include "util/jxx.util.Formatter.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.IntSummaryStatistics.h"
#include "util/jxx.util.ListResourceBundle.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.LongSummaryStatistics.h"
#include "util/jxx.util.Objects.h"
#include "util/jxx.util.Observable.h"
#include "util/jxx.util.Observer.h"
#include "util/jxx.util.OptionalDouble.h"
#include "util/jxx.util.OptionalInt.h"
#include "util/jxx.util.OptionalLong.h"
#include "util/jxx.util.Properties.h"
#include "util/jxx.util.PropertyResourceBundle.h"
#include "util/jxx.util.Random.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.ResourceBundle.h"
#include "util/jxx.util.Scanner.h"
#include "util/jxx.util.Spliterators.h"
#include "util/jxx.util.SplittableRandom.h"
#include "util/jxx.util.StringJoiner.h"
#include "util/jxx.util.StringTokenizer.h"
#include "util/jxx.util.TimeZone.h"
#include "util/jxx.util.Timer.h"
#include "util/jxx.util.TimerTask.h"
#include "util/jxx.util.UUID.h"
namespace jxx::util {
void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
            ::jxx::lang::class_info_detail::registerClassInfo<Arrays>();
            ::jxx::lang::class_info_detail::registerClassInfo<Base64>();
            ::jxx::lang::class_info_detail::registerClassInfo<BitSet>();
            ::jxx::lang::class_info_detail::registerClassInfo<Calendar>();
            ::jxx::lang::class_info_detail::registerClassInfo<Collections>();
            ::jxx::lang::class_info_detail::registerClassInfo<Currency>();
            ::jxx::lang::class_info_detail::registerClassInfo<Date>();
            ::jxx::lang::class_info_detail::registerClassInfo<DoubleSummaryStatistics>();
            ::jxx::lang::class_info_detail::registerClassInfo<EventListener>();
            ::jxx::lang::class_info_detail::registerClassInfo<EventObject>();
            ::jxx::lang::class_info_detail::registerClassInfo<Formattable>();
            ::jxx::lang::class_info_detail::registerClassInfo<FormattableFlags>();
            ::jxx::lang::class_info_detail::registerClassInfo<Formatter>();
            ::jxx::lang::class_info_detail::registerClassInfo<GregorianCalendar>();
            ::jxx::lang::class_info_detail::registerClassInfo<IntSummaryStatistics>();
            ::jxx::lang::class_info_detail::registerClassInfo<ListResourceBundle>();
            ::jxx::lang::class_info_detail::registerClassInfo<Locale>();
            ::jxx::lang::class_info_detail::registerClassInfo<LongSummaryStatistics>();
            ::jxx::lang::class_info_detail::registerClassInfo<Objects>();
            ::jxx::lang::class_info_detail::registerClassInfo<Observable>();
            ::jxx::lang::class_info_detail::registerClassInfo<Observer>();
            ::jxx::lang::class_info_detail::registerClassInfo<OptionalDouble>();
            ::jxx::lang::class_info_detail::registerClassInfo<OptionalInt>();
            ::jxx::lang::class_info_detail::registerClassInfo<OptionalLong>();
            ::jxx::lang::class_info_detail::registerClassInfo<Properties>();
            ::jxx::lang::class_info_detail::registerClassInfo<PropertyResourceBundle>();
            ::jxx::lang::class_info_detail::registerClassInfo<Random>();
            ::jxx::lang::class_info_detail::registerClassInfo<RandomAccess>();
            ::jxx::lang::class_info_detail::registerClassInfo<ResourceBundle>();
            ::jxx::lang::class_info_detail::registerClassInfo<Scanner>();
            ::jxx::lang::class_info_detail::registerClassInfo<Spliterators>();
            ::jxx::lang::class_info_detail::registerClassInfo<SplittableRandom>();
            ::jxx::lang::class_info_detail::registerClassInfo<StringJoiner>();
            ::jxx::lang::class_info_detail::registerClassInfo<StringTokenizer>();
            ::jxx::lang::class_info_detail::registerClassInfo<TimeZone>();
            ::jxx::lang::class_info_detail::registerClassInfo<Timer>();
            ::jxx::lang::class_info_detail::registerClassInfo<TimerTask>();
            ::jxx::lang::class_info_detail::registerClassInfo<UUID>();
        class_initialization_detail::initializeExceptions();
        class_initialization_detail::initializeCollections();
        class_initialization_detail::initializeMaps();
        class_initialization_detail::initializeDateTime();
        class_initialization_detail::initializeLocaleResources();
        class_initialization_detail::initializeUtilities();
    });
}

void initialize()
{
    initializeClasses();
    ::jxx::util::concurrent::initialize();
    ::jxx::util::function::initialize();
    ::jxx::util::logging::initialize();
    ::jxx::util::regex::initialize();
    ::jxx::util::stream::initialize();
    ::jxx::util::zip::initialize();
    ::jxx::util::jar::initialize();
}

}
