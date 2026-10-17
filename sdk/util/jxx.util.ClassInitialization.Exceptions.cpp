#include "util/jxx.util.ClassInitialization.Detail.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/concurrent/jxx.util.concurrent.BrokenBarrierException.h"
#include "util/concurrent/jxx.util.concurrent.CancellationException.h"
#include "util/concurrent/jxx.util.concurrent.CompletionException.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.TimeoutException.h"
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

namespace jxx::util::class_initialization_detail {
namespace {
template <typename T>
void registerType() { ::jxx::lang::class_info_detail::registerClassInfo<T>(); }
}
void initializeExceptions() {
    static std::once_flag flag;
    std::call_once(flag, [] {
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
        registerType<concurrent::BrokenBarrierException>();
        registerType<concurrent::CancellationException>();
        registerType<concurrent::CompletionException>();
        registerType<concurrent::ExecutionException>();
        registerType<concurrent::RejectedExecutionException>();
        registerType<concurrent::TimeoutException>();
    });
}
}
