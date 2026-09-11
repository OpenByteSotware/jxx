#pragma once

#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DeclHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.EntityResolver2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.LexicalHandler.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"

namespace jxx::org::xml::sax::ext {

class DefaultHandler2
    : public ::jxx::lang::ClassBase<
          DefaultHandler2,
          ::jxx::org::xml::sax::helpers::DefaultHandler,
          DeclHandler,
          EntityResolver2,
          LexicalHandler> {
public:
    using JxxSuper =
        ::jxx::org::xml::sax::helpers::DefaultHandler;

    using Super = ::jxx::lang::ClassBase<
        DefaultHandler2,
        JxxSuper,
        DeclHandler,
        EntityResolver2,
        LexicalHandler>;

    DefaultHandler2();
    ~DefaultHandler2() override;

    void elementDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& model) override;

    void attributeDecl(
        const ::jxx::Ptr<::jxx::lang::String>& elementName,
        const ::jxx::Ptr<::jxx::lang::String>& attributeName,
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& mode,
        const ::jxx::Ptr<::jxx::lang::String>& value) override;

    void internalEntityDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& value) override;

    void externalEntityDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    ::jxx::Ptr<::jxx::org::xml::sax::InputSource>
    getExternalSubset(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& baseURI) override;

    ::jxx::Ptr<::jxx::org::xml::sax::InputSource>
    resolveEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& baseURI,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    ::jxx::Ptr<::jxx::org::xml::sax::InputSource>
    resolveEntity(
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    void startDTD(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;
    void endDTD() override;
    void startEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name) override;
    void endEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name) override;
    void startCDATA() override;
    void endCDATA() override;
    void comment(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) override;
};

} // namespace jxx::org::xml::sax::ext
