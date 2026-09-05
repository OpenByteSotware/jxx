#include "util/jxx.util.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
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
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.TooManyListenersException.h"
#include "util/jxx.util.UnknownFormatConversionException.h"
#include "util/jxx.util.UnknownFormatFlagsException.h"
#include "util/jxx.util.Properties.h"
#include "util/jxx.util.concurrent.BrokenBarrierException.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.CompletionException.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

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
        });
}

} // namespace jxx::util
