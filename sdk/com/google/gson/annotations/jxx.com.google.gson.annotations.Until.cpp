#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Until.h"

namespace com::google::gson::annotations {
Until::Until(::jxx::lang::jdouble value) : value_(value) {}
::jxx::lang::jdouble Until::value() const noexcept { return value_; }
} // namespace com::google::gson::annotations
