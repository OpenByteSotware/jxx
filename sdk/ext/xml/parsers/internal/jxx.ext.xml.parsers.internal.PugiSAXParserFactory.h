#pragma once
#include <string>
#include <unordered_map>
#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParserFactory.h"
namespace jxx::ext::xml::parsers::internal {class PugiSAXParserFactory final:public ::jxx::ext::xml::parsers::SAXParserFactory {public:PugiSAXParserFactory();~PugiSAXParserFactory()override;::jxx::Ptr<::jxx::ext::xml::parsers::SAXParser> newSAXParser()override;void setFeature(const ::jxx::Ptr<::jxx::lang::String>& name,::jxx::lang::jbool value)override;::jxx::lang::jbool getFeature(const ::jxx::Ptr<::jxx::lang::String>& name)const override;private:std::unordered_map<std::string,::jxx::lang::jbool> features_;};}
