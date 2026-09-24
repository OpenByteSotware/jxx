#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapter.h"

namespace jxx::ext::xml::bind::annotation::adapters {
XmlJavaTypeAdapter::XmlJavaTypeAdapter(const ::jxx::Ptr<::jxx::lang::ClassAny>&v,const ::jxx::Ptr<::jxx::lang::ClassAny>&t):value_(v),type_(t){} ::jxx::Ptr<::jxx::lang::ClassAny> XmlJavaTypeAdapter::value()const{return value_;} ::jxx::Ptr<::jxx::lang::ClassAny> XmlJavaTypeAdapter::type()const{return type_;}
}
