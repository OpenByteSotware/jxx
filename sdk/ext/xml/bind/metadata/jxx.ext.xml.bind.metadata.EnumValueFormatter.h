#pragma once
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueMap.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueFormatter.h"
namespace jxx::ext::xml::bind::metadata {
class EnumValueFormatter final
    : public ::jxx::lang::ClassBase<
          EnumValueFormatter,
          ::jxx::lang::Object,
          ValueFormatter> {
public:
    explicit EnumValueFormatter(const ::jxx::Ptr<EnumValueMap>& values);
    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
private:
    ::jxx::Ptr<EnumValueMap> values_;
};
} // namespace jxx::ext::xml::bind::metadata
