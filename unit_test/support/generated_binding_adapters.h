#pragma once

#include "lang/jxx.lang.Object.h"

namespace com::google::gson { class TypeAdapter; }
namespace jxx::ext::xml::bind::metadata { class PropertyBinding; }

namespace jxx::test::generated {

::jxx::Ptr<::com::google::gson::TypeAdapter>
portJsonAdapter();

::jxx::Ptr<::jxx::lang::Object>
newPortModel(::jxx::lang::jint port);

::jxx::lang::jint
portFromModel(
    const ::jxx::Ptr<::jxx::lang::Object>& model);

::jxx::Ptr<
    ::jxx::ext::xml::bind::metadata::PropertyBinding>
portXmlBinding();

} // namespace jxx::test::generated
