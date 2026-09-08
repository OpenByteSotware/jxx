#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::lang { class String; }
namespace jxx::org::xml::sax { class InputSource; class SAXParseException; class EntityResolver : public jxx::lang::InterfaceBase<EntityResolver> { public: ~EntityResolver() override=default; virtual jxx::Ptr<InputSource> resolveEntity(const jxx::Ptr<jxx::lang::String>& publicId,const jxx::Ptr<jxx::lang::String>& systemId)=0; }; }
