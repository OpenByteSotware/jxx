#include "com/google/gson/internal/jxx.com.google.gson.internal.ClassInitialization.h"

#include <mutex>

#include "com/google/gson/internal/jxx.com.google.gson.internal.GsonContext.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeReader.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeWriter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.NullSafeTypeAdapter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.Streams.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.StringWriter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.TreeTypeAdapter.h"

namespace com::google::gson::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)GsonContext::JxxClassInfoMarker::Class();
        (void)JsonTreeReader::JxxClassInfoMarker::Class();
        (void)JsonTreeWriter::JxxClassInfoMarker::Class();
        (void)NullSafeTypeAdapter::JxxClassInfoMarker::Class();
        (void)Streams::JxxClassInfoMarker::Class();
        (void)StringWriter::JxxClassInfoMarker::Class();
        (void)TreeTypeAdapter::JxxClassInfoMarker::Class();
    });
}

} // namespace com::google::gson::internal
