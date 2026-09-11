#include <gtest/gtest.h>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.CDATASection.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Comment.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
TEST(XmlDomLsSerializerConfigurationBehaviorStage, OmitsCommentsWhenDisabled) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()->createDocument(
        nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto root = document->getDocumentElement();
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createComment(::jxx::NEW<::jxx::lang::String>("hidden"))));
    const auto serializer = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    serializer->getDomConfig()->setParameter(
        ::jxx::NEW<::jxx::lang::String>("comments"),
        ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(false)));
    EXPECT_EQ(serializer->writeToString(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(document))->utf8(), "<root/>");
}

TEST(XmlDomLsSerializerConfigurationBehaviorStage, ConvertsCdataWhenDisabled) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()->createDocument(
        nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto root = document->getDocumentElement();
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createCDATASection(::jxx::NEW<::jxx::lang::String>("x < y"))));
    const auto serializer = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    serializer->getDomConfig()->setParameter(
        ::jxx::NEW<::jxx::lang::String>("cdata-sections"),
        ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(false)));
    EXPECT_EQ(serializer->writeToString(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(document))->utf8(),
        "<root>x &lt; y</root>");
}
