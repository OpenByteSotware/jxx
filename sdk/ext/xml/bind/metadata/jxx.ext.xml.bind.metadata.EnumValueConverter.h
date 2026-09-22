#pragma once
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueMap.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"
namespace jxx::ext::xml::bind::metadata {
class EnumValueConverter final
    : public ::jxx::lang::ClassBase<
          EnumValueConverter,
          ::jxx::lang::Object,
          ValueConverter> {
public:
    explicit EnumValueConverter(const ::jxx::Ptr<EnumValueMap>& values);
    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;
private:
    ::jxx::Ptr<EnumValueMap> values_;
};
} // namespace jxx::ext::xml::bind::metadata
