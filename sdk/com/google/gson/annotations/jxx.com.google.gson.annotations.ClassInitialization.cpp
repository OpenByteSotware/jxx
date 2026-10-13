#include "com/google/gson/annotations/jxx.com.google.gson.annotations.ClassInitialization.h"

#include <mutex>

#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Expose.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.JsonAdapter.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.SerializedName.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Since.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Until.h"

namespace com::google::gson::annotations {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)Expose::JxxClassInfoMarker::Class();
        (void)JsonAdapter::JxxClassInfoMarker::Class();
        (void)SerializedName::JxxClassInfoMarker::Class();
        (void)Since::JxxClassInfoMarker::Class();
        (void)Until::JxxClassInfoMarker::Class();
    });
}

} // namespace com::google::gson::annotations
