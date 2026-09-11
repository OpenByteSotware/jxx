#include <gtest/gtest.h>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

namespace {
void setFlag(const ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>& config,
    const char* name, ::jxx::lang::jbool value) {
    config->setParameter(::jxx::NEW<::jxx::lang::String>(name),
        ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(value)));
}
}

TEST(XmlDomLsSerializerFormattingStage, DocumentIncludesDeclarationByDefault) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()->createDocument(
        nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto serializer = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    EXPECT_EQ(serializer->writeToString(::jxx::CAST<::jxx::org::w3c::dom::Node>(document))->utf8(),
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?><root/>");
}

TEST(XmlDomLsSerializerFormattingStage, DeclarationCanBeDisabled) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()->createDocument(
        nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto serializer = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    setFlag(serializer->getDomConfig(), "xml-declaration", false);
    EXPECT_EQ(serializer->writeToString(::jxx::CAST<::jxx::org::w3c::dom::Node>(document))->utf8(), "<root/>");
}

TEST(XmlDomLsSerializerFormattingStage, PrettyPrintUsesConfiguredNewLine) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()->createDocument(
        nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto root = document->getDocumentElement();
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createElement(::jxx::NEW<::jxx::lang::String>("child"))));
    const auto serializer = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    setFlag(serializer->getDomConfig(), "xml-declaration", false);
    setFlag(serializer->getDomConfig(), "format-pretty-print", true);
    serializer->setNewLine(::jxx::NEW<::jxx::lang::String>("\r\n"));
    EXPECT_EQ(serializer->writeToString(::jxx::CAST<::jxx::org::w3c::dom::Node>(document))->utf8(),
        "<root>\r\n  <child/>\r\n</root>");
}
