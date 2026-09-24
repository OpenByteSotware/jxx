#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAccessorType.h"

namespace jxx::ext::xml::bind::annotation {
XmlAccessorType::XmlAccessorType(XmlAccessType v):Super(),value_(v){} XmlAccessType XmlAccessorType::value()const noexcept{return value_;}
}
