#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace com::google::gson::annotations {
class SerializedName final
    : public ::jxx::lang::ClassBase<SerializedName, ::jxx::lang::Object> {
public:
    using StringArray = ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>;
    SerializedName(
        const ::jxx::Ptr<::jxx::lang::String>& value,
        const ::jxx::Ptr<StringArray>& alternate = nullptr);
    ::jxx::Ptr<::jxx::lang::String> value() const;
    ::jxx::Ptr<StringArray> alternate() const;
private:
    ::jxx::Ptr<::jxx::lang::String> value_;
    ::jxx::Ptr<StringArray> alternate_;
};
} // namespace com::google::gson::annotations
