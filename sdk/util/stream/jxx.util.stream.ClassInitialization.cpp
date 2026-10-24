#include <mutex>
#include "util/stream/jxx.util.stream.ClassInitialization.h"
#include "util/stream/jxx.util.stream.IntStream.h"
#include "util/stream/jxx.util.stream.LongStream.h"
#include "util/stream/jxx.util.stream.DoubleStream.h"
namespace jxx::util::stream {
void initialize() { static std::once_flag flag; std::call_once(flag, [] { IntStream::Class(); LongStream::Class(); DoubleStream::Class(); }); }
}
