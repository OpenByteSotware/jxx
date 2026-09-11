#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class Object;
class String;
}

namespace jxx::org::xml::sax {

class ContentHandler;
class DTDHandler;
class EntityResolver;
class ErrorHandler;
class InputSource;

class XMLReader
    : public ::jxx::lang::InterfaceBase<XMLReader> {
public:
    ~XMLReader() override = default;

    virtual ::jxx::lang::jbool getFeature(
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;

    virtual void setFeature(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        ::jxx::lang::jbool value) = 0;

    virtual ::jxx::Ptr<::jxx::lang::Object> getProperty(
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;

    virtual void setProperty(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;

    virtual void setEntityResolver(
        const ::jxx::Ptr<EntityResolver>& resolver) = 0;

    virtual ::jxx::Ptr<EntityResolver> getEntityResolver() const = 0;

    virtual void setDTDHandler(
        const ::jxx::Ptr<DTDHandler>& handler) = 0;

    virtual ::jxx::Ptr<DTDHandler> getDTDHandler() const = 0;

    virtual void setContentHandler(
        const ::jxx::Ptr<ContentHandler>& handler) = 0;

    virtual ::jxx::Ptr<ContentHandler> getContentHandler() const = 0;

    virtual void setErrorHandler(
        const ::jxx::Ptr<ErrorHandler>& handler) = 0;

    virtual ::jxx::Ptr<ErrorHandler> getErrorHandler() const = 0;

    virtual void parse(
        const ::jxx::Ptr<InputSource>& input) = 0;

    virtual void parse(
        const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;
};

} // namespace jxx::org::xml::sax
