#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
namespace jxx::ext::xml::transform {
namespace { ::jxx::Ptr<::jxx::lang::String> key(const char* value) { return ::jxx::NEW<::jxx::lang::String>(value); } }
::jxx::Ptr<::jxx::lang::String> OutputKeys::METHOD(){ static const auto v=key("method"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::VERSION(){ static const auto v=key("version"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::ENCODING(){ static const auto v=key("encoding"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::OMIT_XML_DECLARATION(){ static const auto v=key("omit-xml-declaration"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::STANDALONE(){ static const auto v=key("standalone"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::DOCTYPE_PUBLIC(){ static const auto v=key("doctype-public"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::DOCTYPE_SYSTEM(){ static const auto v=key("doctype-system"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::CDATA_SECTION_ELEMENTS(){ static const auto v=key("cdata-section-elements"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::INDENT(){ static const auto v=key("indent"); return v; }
::jxx::Ptr<::jxx::lang::String> OutputKeys::MEDIA_TYPE(){ static const auto v=key("media-type"); return v; }
}
