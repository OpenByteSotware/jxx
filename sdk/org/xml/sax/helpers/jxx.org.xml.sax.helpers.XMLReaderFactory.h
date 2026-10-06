#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::lang { class String; }
namespace jxx::org::xml::sax { class XMLReader; }
namespace jxx::org::xml::sax::helpers {
class XMLReaderFactory final : public ::jxx::lang::ClassBase<XMLReaderFactory, ::jxx::lang::Object> {
public:
 static ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> createXMLReader();
 static ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> createXMLReader(const ::jxx::Ptr<::jxx::lang::String>& className);
private: XMLReaderFactory()=delete;
};
} // namespace jxx::org::xml::sax::helpers
