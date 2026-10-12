#include "nio/jxx.nio.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "nio/jxx.nio.Buffer.h"
#include "nio/jxx.nio.BufferOverflowException.h"
#include "nio/jxx.nio.BufferUnderflowException.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/jxx.nio.ByteOrder.h"
#include "nio/jxx.nio.CharBuffer.h"
#include "nio/jxx.nio.DoubleBuffer.h"
#include "nio/jxx.nio.FloatBuffer.h"
#include "nio/jxx.nio.IntBuffer.h"
#include "nio/jxx.nio.InvalidMarkException.h"
#include "nio/jxx.nio.LongBuffer.h"
#include "nio/jxx.nio.MappedByteBuffer.h"
#include "nio/jxx.nio.ShortBuffer.h"

namespace jxx::nio {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<Buffer>();
        registerType<BufferOverflowException>();
        registerType<BufferUnderflowException>();
        registerType<ByteBuffer>();
        registerType<ByteOrder>();
        registerType<CharBuffer>();
        registerType<DoubleBuffer>();
        registerType<FloatBuffer>();
        registerType<IntBuffer>();
        registerType<InvalidMarkException>();
        registerType<LongBuffer>();
        registerType<MappedByteBuffer>();
        registerType<ShortBuffer>();
    });
}

} // namespace jxx::nio
