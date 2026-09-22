#pragma once

#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"

namespace jxx::ext::xml::bind::metadata {

class AdaptedValueConverter final
    : public ::jxx::lang::ClassBase<
          AdaptedValueConverter,
          ::jxx::lang::Object,
          ValueConverter> {
public:
    AdaptedValueConverter(
        const ::jxx::Ptr<ValueConverter>& lexicalConverter,
        const ::jxx::Ptr<
            ::jxx::ext::xml::bind::annotation::adapters::XmlAdapter>& adapter);

    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;

private:
    ::jxx::Ptr<ValueConverter> lexicalConverter_;
    ::jxx::Ptr<
        ::jxx::ext::xml::bind::annotation::adapters::XmlAdapter> adapter_;
};

} // namespace jxx::ext::xml::bind::metadata
