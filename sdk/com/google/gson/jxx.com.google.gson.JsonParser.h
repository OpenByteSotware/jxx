#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Reader.h"

namespace jxx::lang { class String; }
namespace com::google::gson { class JsonElement; }

namespace com::google::gson {

class JsonParser final
    : public ::jxx::lang::ClassBase<
          JsonParser,
          ::jxx::lang::Object> {
public:
    static ::jxx::Ptr<JsonElement> parseString(
        const ::jxx::Ptr<::jxx::lang::String>& json);
    static ::jxx::Ptr<JsonElement> parseReader(
        const ::jxx::Ptr<::jxx::io::Reader>& reader);
};

} // namespace com::google::gson
