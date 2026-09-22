#include "com/google/gson/annotations/jxx.com.google.gson.annotations.SerializedName.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson::annotations {
SerializedName::SerializedName(
    const ::jxx::Ptr<::jxx::lang::String>& value,
    const ::jxx::Ptr<StringArray>& alternate)
    : value_(value), alternate_(alternate) {
    if (value_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> SerializedName::value() const { return value_; }
::jxx::Ptr<SerializedName::StringArray> SerializedName::alternate() const { return alternate_; }
} // namespace com::google::gson::annotations
