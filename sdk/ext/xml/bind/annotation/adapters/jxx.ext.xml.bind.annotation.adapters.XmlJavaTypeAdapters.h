#pragma once

#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapter.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::xml::bind::annotation::adapters {

class XmlJavaTypeAdapters final
    : public ::jxx::lang::ClassBase<
          XmlJavaTypeAdapters,
          ::jxx::lang::Object> {
public:
    using AdapterArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<XmlJavaTypeAdapter>, 1U>;

    explicit XmlJavaTypeAdapters(
        const ::jxx::Ptr<AdapterArray>& value)
        : value_(value) {}

    ::jxx::Ptr<AdapterArray> value() const { return value_; }

private:
    ::jxx::Ptr<AdapterArray> value_;
};

} // namespace jxx::ext::xml::bind::annotation::adapters
