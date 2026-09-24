#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementRefs.h"

namespace jxx::ext::xml::bind::annotation {
XmlElementRefs::XmlElementRefs(const ::jxx::Ptr<RefArray>&v):value_(v){} ::jxx::Ptr<XmlElementRefs::RefArray> XmlElementRefs::value()const{return value_;}
}
