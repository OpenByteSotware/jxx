#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElements.h"

namespace jxx::ext::xml::bind::annotation {
XmlElements::XmlElements(const ::jxx::Ptr<ElementArray>&v):value_(v){} ::jxx::Ptr<XmlElements::ElementArray> XmlElements::value()const{return value_;}
}
