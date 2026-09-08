#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::org::w3c::dom { DOMException::DOMException(jxx::lang::jshort c,const jxx::Ptr<jxx::lang::String>& m):jxx::lang::RuntimeException(m),code(c){} const char* DOMException::typeName() const noexcept{return "DOMException";} jxx::Ptr<jxx::lang::Object> DOMException::cloneImpl() const{return jxx::CAST<jxx::lang::Object>(jxx::NEW<DOMException>(*this));} }
