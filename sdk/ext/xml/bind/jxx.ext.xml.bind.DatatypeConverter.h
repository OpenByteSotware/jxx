#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx_types.h"

namespace jxx::lang { class String; }

namespace jxx::ext::xml::bind {

class DatatypeConverter final
    : public ::jxx::lang::ClassBase<
          DatatypeConverter,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<DatatypeConverter, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::String> printHexBinary(
        const ::jxx::lang::ByteArray& value);
    static ::jxx::lang::ByteArray parseHexBinary(
        const ::jxx::Ptr<::jxx::lang::String>& lexicalValue);

private:
    DatatypeConverter() = delete;
};

} // namespace jxx::ext::xml::bind
