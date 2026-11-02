#include "util/stream/jxx.util.stream.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "util/stream/jxx.util.stream.BaseStream.h"
#include "util/stream/jxx.util.stream.DoubleStream.h"
#include "util/stream/jxx.util.stream.IntStream.h"
#include "util/stream/jxx.util.stream.LongStream.h"

namespace jxx::util::stream {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        using IntBaseStream = BaseStream<::jxx::lang::Integer, IntStream>;
        using LongBaseStream = BaseStream<::jxx::lang::Long, LongStream>;
        using DoubleBaseStream = BaseStream<::jxx::lang::Double, DoubleStream>;

        (void)IntBaseStream::Class();
        (void)LongBaseStream::Class();
        (void)DoubleBaseStream::Class();
        (void)IntStream::Class();
        (void)LongStream::Class();
        (void)DoubleStream::Class();
    });
}

void initialize()
{
    initializeClasses();
}

} // namespace jxx::util::stream
