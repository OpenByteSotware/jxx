#pragma once
#include <memory>
#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"
namespace jxx::org::xml::sax::internal {
class PugiXMLReader final : public ::jxx::lang::ClassBase<PugiXMLReader,::jxx::lang::Object,::jxx::org::xml::sax::XMLReader> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<PugiXMLReader,JxxSuper,::jxx::org::xml::sax::XMLReader>;
 PugiXMLReader(); ~PugiXMLReader() override;
 ::jxx::lang::jbool getFeature(const ::jxx::Ptr<::jxx::lang::String>& name)const override;
 void setFeature(const ::jxx::Ptr<::jxx::lang::String>& name,::jxx::lang::jbool value) override;
 ::jxx::Ptr<::jxx::lang::Object> getProperty(const ::jxx::Ptr<::jxx::lang::String>& name)const override;
 void setProperty(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value) override;
 void setEntityResolver(const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& v)override; ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> getEntityResolver()const override;
 void setDTDHandler(const ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>& v)override; ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> getDTDHandler()const override;
 void setContentHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>& v)override; ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> getContentHandler()const override;
 void setErrorHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& v)override; ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> getErrorHandler()const override;
 void parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input)override;
 void parse(const ::jxx::Ptr<::jxx::lang::String>& systemId)override;
private: struct State; std::unique_ptr<State> state_;
};
} // namespace jxx::org::xml::sax::internal
