#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::lang { class String; }
namespace jxx::org::xml::sax {
class AttributeList;
class Locator;

class DocumentHandler
    : public ::jxx::lang::InterfaceBase<DocumentHandler> {
public:
    ~DocumentHandler() override = default;
    virtual void setDocumentLocator(const ::jxx::Ptr<Locator>& locator) = 0;
    virtual void startDocument() = 0;
    virtual void endDocument() = 0;
    virtual void startElement(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<AttributeList>& attributes) = 0;
    virtual void endElement(const ::jxx::Ptr<::jxx::lang::String>& name) = 0;
    virtual void characters(const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start, ::jxx::lang::jint length) = 0;
    virtual void ignorableWhitespace(const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start, ::jxx::lang::jint length) = 0;
    virtual void processingInstruction(
        const ::jxx::Ptr<::jxx::lang::String>& target,
        const ::jxx::Ptr<::jxx::lang::String>& data) = 0;
};

} // namespace jxx::org::xml::sax
