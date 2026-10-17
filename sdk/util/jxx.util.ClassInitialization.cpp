#include "util/jxx.util.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.DuplicateFormatFlagsException.h"
#include "util/jxx.util.EmptyStackException.h"
#include "util/jxx.util.FormatFlagsConversionMismatchException.h"
#include "util/jxx.util.FormatterClosedException.h"
#include "util/jxx.util.IllegalFormatCodePointException.h"
#include "util/jxx.util.IllegalFormatConversionException.h"
#include "util/jxx.util.IllegalFormatException.h"
#include "util/jxx.util.IllegalFormatFlagsException.h"
#include "util/jxx.util.IllegalFormatPrecisionException.h"
#include "util/jxx.util.IllegalFormatWidthException.h"
#include "util/jxx.util.InputMismatchException.h"
#include "util/jxx.util.MissingFormatArgumentException.h"
#include "util/jxx.util.MissingResourceException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.TooManyListenersException.h"
#include "util/jxx.util.UnknownFormatConversionException.h"
#include "util/jxx.util.UnknownFormatFlagsException.h"
#include "util/jxx.util.Properties.h"
#include "util/concurrent/jxx.util.concurrent.BrokenBarrierException.h"
#include "util/concurrent/jxx.util.concurrent.CancellationException.h"
#include "util/concurrent/jxx.util.concurrent.CompletionException.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.TimeoutException.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Base64.h"
#include "util/jxx.util.BitSet.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.Collections.h"
#include "util/jxx.util.Currency.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.DoubleSummaryStatistics.h"
#include "util/jxx.util.EventListener.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.LinkedHashSet.h"
#include "util/jxx.util.LinkedList.h"
#include "util/jxx.util.EventObject.h"
#include "util/jxx.util.Formattable.h"
#include "util/jxx.util.Formatter.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.IntStream.h"
#include "util/jxx.util.IntSummaryStatistics.h"
#include "util/jxx.util.ListResourceBundle.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.LocaleBuilder.h"
#include "util/jxx.util.LongSummaryStatistics.h"
#include "util/jxx.util.Objects.h"
#include "util/jxx.util.Observable.h"
#include "util/jxx.util.Observer.h"
#include "util/jxx.util.OptionalDouble.h"
#include "util/jxx.util.OptionalInt.h"
#include "util/jxx.util.OptionalLong.h"
#include "util/jxx.util.PluralRules.h"
#include "util/jxx.util.PropertyResourceBundle.h"
#include "util/jxx.util.Random.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.ResourceBundle.h"
#include "util/jxx.util.Scanner.h"
#include "util/jxx.util.SimpleTimeZone.h"
#include "util/jxx.util.Spliterators.h"
#include "util/jxx.util.SplittableRandom.h"
#include "util/jxx.util.StringJoiner.h"
#include "util/jxx.util.Timer.h"
#include "util/jxx.util.TimerTask.h"
#include "util/jxx.util.TimeZone.h"
#include "util/jxx.util.UUID.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.ZonedDateTime.h"
namespace jxx::util {

namespace {

template <typename T>
void registerType()
{
    jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;

    std::call_once(
        flag,
        [] {
            registerType<ConcurrentModificationException>();
            registerType<DuplicateFormatFlagsException>();
            registerType<EmptyStackException>();
            registerType<FormatFlagsConversionMismatchException>();
            registerType<FormatterClosedException>();
            registerType<IllegalFormatCodePointException>();
            registerType<IllegalFormatConversionException>();
            registerType<IllegalFormatException>();
            registerType<IllegalFormatFlagsException>();
            registerType<IllegalFormatPrecisionException>();
            registerType<IllegalFormatWidthException>();
            registerType<InputMismatchException>();
            registerType<MissingFormatArgumentException>();
            registerType<MissingResourceException>();
            registerType<NoSuchElementException>();
            registerType<TooManyListenersException>();
            registerType<UnknownFormatConversionException>();
            registerType<UnknownFormatFlagsException>();
            registerType<Properties>();
            registerType<concurrent::BrokenBarrierException>();
            registerType<concurrent::CancellationException>();
            registerType<concurrent::CompletionException>();
            registerType<concurrent::ExecutionException>();
            registerType<concurrent::RejectedExecutionException>();
            registerType<concurrent::TimeoutException>();
        registerType<Arrays>();
        registerType<ArrayList<::jxx::lang::Object>>();
        ::jxx::io::serialization::registerSerializableClass<
            ArrayList<::jxx::lang::Object>>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            ArrayList<::jxx::lang::Object>, 1U, 2U>();
        registerType<Base64>();
        registerType<BitSet>();
        ::jxx::io::serialization::registerSerializableClass<BitSet>(1);
        registerType<Calendar>();
        ::jxx::io::serialization::registerSerializableClass<Calendar>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            Calendar, 1U, 2U>();
        registerType<Collections>();
        registerType<Currency>();
        ::jxx::io::serialization::registerSerializableClass<Currency>(1, [] { return ::jxx::NEW<Currency>(::jxx::NEW<::jxx::lang::String>("XXX"), ::jxx::NEW<::jxx::lang::String>("XXX"), 999, 2); });
        registerType<Date>();
        ::jxx::io::serialization::registerSerializableClass<Date>(1);
        registerType<DoubleSummaryStatistics>();
        registerType<EventListener>();
        registerType<EventObject>();
        ::jxx::io::serialization::registerSerializableClass<EventObject>(1, [] { return ::jxx::NEW<EventObject>(::jxx::NEW<::jxx::lang::Object>()); });
        registerType<Formattable>();
        registerType<Formatter>();
        registerType<GregorianCalendar>();
        ::jxx::io::serialization::registerSerializableClass<GregorianCalendar>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<GregorianCalendar, 1U, 2U>();
        using ErasedHashMap = HashMap<::jxx::lang::Object, ::jxx::lang::Object>;
        registerType<ErasedHashMap>();
        ::jxx::io::serialization::registerSerializableClass<ErasedHashMap>(
            static_cast<::jxx::lang::jlong>(362498820763181265LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedHashMap, 1U, 2U>();
        using ErasedLinkedList = LinkedList<::jxx::lang::Object>;
        registerType<ErasedLinkedList>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedList>(
            static_cast<::jxx::lang::jlong>(876323262645176354LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedList, 1U, 2U>();
        using ErasedLinkedHashSet = LinkedHashSet<::jxx::lang::Object>;
        registerType<ErasedLinkedHashSet>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedHashSet>(
            static_cast<::jxx::lang::jlong>(-2851667679971038690LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedHashSet, 1U, 2U>();
        registerType<IntStream>();
        ::jxx::io::serialization::registerSerializableClass<IntStream>(1, [] { return IntStream::of(::jxx::NEW<::jxx::lang::IntArrayType>(0)); });
        registerType<IntSummaryStatistics>();
        registerType<ListResourceBundle>();
        registerType<Locale>();
        ::jxx::io::serialization::registerSerializableClass<Locale>(1, [] {
            return ::jxx::NEW<Locale>(::jxx::NEW<::jxx::lang::String>(""));
        });
        registerType<LocaleBuilder>();
        registerType<LongSummaryStatistics>();
        registerType<Objects>();
        registerType<Observable>();
        registerType<Observer>();
        registerType<OptionalDouble>();
        registerType<OptionalInt>();
        registerType<OptionalLong>();
        registerType<PluralRules>();
        registerType<PropertyResourceBundle>();
        registerType<Random>();
        ::jxx::io::serialization::registerSerializableClass<Random>(1);
        registerType<RandomAccess>();
        registerType<ResourceBundle>();
        registerType<Scanner>();
        registerType<SimpleTimeZone>();
        registerType<Spliterators>();
        registerType<SplittableRandom>();
        registerType<StringJoiner>();
        registerType<Timer>();
        registerType<TimerTask>();
        registerType<TimeZone>();
        registerType<UUID>();
        ::jxx::io::serialization::registerSerializableClass<UUID>(1, [] {
            return ::jxx::NEW<UUID>(0, 0);
        });
        registerType<::jxx::util::wildcard::CollectionAny>();
        registerType<ZonedDateTime>();
        });
}

} // namespace jxx::util
