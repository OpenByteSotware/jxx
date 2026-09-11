#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang { class String; }
namespace jxx::util { class Locale; }
namespace jxx::org::xml::sax {
class DTDHandler;
class DocumentHandler;
class EntityResolver;
class ErrorHandler;
class InputSource;

class Parser
    : public ::jxx::lang::InterfaceBase<Parser> {
public:
    ~Parser() override = default;
    virtual void setLocale(const ::jxx::Ptr<::jxx::util::Locale>& locale) = 0;
    virtual void setEntityResolver(const ::jxx::Ptr<EntityResolver>& resolver) = 0;
    virtual void setDTDHandler(const ::jxx::Ptr<DTDHandler>& handler) = 0;
    virtual void setDocumentHandler(const ::jxx::Ptr<DocumentHandler>& handler) = 0;
    virtual void setErrorHandler(const ::jxx::Ptr<ErrorHandler>& handler) = 0;
    virtual void parse(const ::jxx::Ptr<InputSource>& source) = 0;
    virtual void parse(const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;
};

} // namespace jxx::org::xml::sax
