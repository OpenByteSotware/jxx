#include "com/google/gson/stream/jxx.com.google.gson.stream.ClassInitialization.h"

#include <mutex>

#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.MalformedJsonException.h"

namespace com::google::gson::stream {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<JsonReader, ::jxx::lang::Object, ::jxx::io::Closeable>::Class();
        (void)::jxx::lang::ClassBase<JsonWriter, ::jxx::lang::Object, ::jxx::io::Closeable, ::jxx::io::Flushable>::Class();
        (void)::jxx::lang::ClassBase<MalformedJsonException, ::jxx::io::IOException>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace com::google::gson::stream
