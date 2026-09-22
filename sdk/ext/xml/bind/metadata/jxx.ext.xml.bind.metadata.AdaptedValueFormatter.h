#pragma once

#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueFormatter.h"

namespace jxx::ext::xml::bind::metadata {

class AdaptedValueFormatter final
    : public ::jxx::lang::ClassBase<
          AdaptedValueFormatter,
          ::jxx::lang::Object,
          ValueFormatter> {
public:
    AdaptedValueFormatter(
        const ::jxx::Ptr<ValueFormatter>& lexicalFormatter,
        const ::jxx::Ptr<
            ::jxx::ext::xml::bind::annotation::adapters::XmlAdapter>& adapter);

    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;

private:
    ::jxx::Ptr<ValueFormatter> lexicalFormatter_;
    ::jxx::Ptr<
        ::jxx::ext::xml::bind::annotation::adapters::XmlAdapter> adapter_;
};

} // namespace jxx::ext::xml::bind::metadata
