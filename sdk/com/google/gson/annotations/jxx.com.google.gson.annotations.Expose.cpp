#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Expose.h"

namespace com::google::gson::annotations {
Expose::Expose(::jxx::lang::jbool serialize, ::jxx::lang::jbool deserialize)
    : serialize_(serialize), deserialize_(deserialize) {}
::jxx::lang::jbool Expose::serialize() const noexcept { return serialize_; }
::jxx::lang::jbool Expose::deserialize() const noexcept { return deserialize_; }
} // namespace com::google::gson::annotations
