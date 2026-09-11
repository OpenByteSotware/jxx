#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::ext::xml::validation { class Schema; }
namespace jxx::lang { class ClassLoader; class String; }
namespace jxx::ext::xml::parsers {
class SAXParser;
class SAXParserFactory
    : public ::jxx::lang::ClassBase<SAXParserFactory, ::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<SAXParserFactory,JxxSuper>;
 virtual ~SAXParserFactory();
 static ::jxx::Ptr<SAXParserFactory> newInstance();
 static ::jxx::Ptr<SAXParserFactory> newInstance(const ::jxx::Ptr<::jxx::lang::String>& factoryClassName,const ::jxx::Ptr<::jxx::lang::ClassLoader>& classLoader);
 virtual ::jxx::Ptr<SAXParser> newSAXParser()=0;
 virtual void setFeature(const ::jxx::Ptr<::jxx::lang::String>& name,::jxx::lang::jbool value)=0;
 virtual ::jxx::lang::jbool getFeature(const ::jxx::Ptr<::jxx::lang::String>& name)const=0;
 void setNamespaceAware(::jxx::lang::jbool awareness); ::jxx::lang::jbool isNamespaceAware()const;
 void setValidating(::jxx::lang::jbool validating); ::jxx::lang::jbool isValidating()const;
 void setSchema(const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& schema); ::jxx::Ptr<::jxx::ext::xml::validation::Schema> getSchema()const;
 void setXIncludeAware(::jxx::lang::jbool state); ::jxx::lang::jbool isXIncludeAware()const;
protected: SAXParserFactory();
private: ::jxx::lang::jbool namespaceAware_=false;::jxx::lang::jbool validating_=false;::jxx::lang::jbool xincludeAware_=false;::jxx::Ptr<::jxx::ext::xml::validation::Schema> schema_;
};
} // namespace jxx::ext::xml::parsers
