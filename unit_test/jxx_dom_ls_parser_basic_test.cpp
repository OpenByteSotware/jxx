#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"

TEST(XmlDomLsParserBasicStage, FactoryCreatesSynchronousParser) {
    const auto parser =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSParser(
                ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
                nullptr);
    ASSERT_NE(parser, nullptr);
    EXPECT_FALSE(parser->getAsync());
    EXPECT_FALSE(parser->getBusy());
}

TEST(XmlDomLsParserBasicStage, ParsesStringData) {
    const auto factory =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto input = factory->createLSInput();
    input->setStringData(
        ::jxx::NEW<::jxx::lang::String>("<root id=\"7\"/>"));
    const auto parser = factory->createLSParser(
        ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
        nullptr);

    const auto document = parser->parse(input);
    ASSERT_NE(document, nullptr);
    ASSERT_NE(document->getDocumentElement(), nullptr);
    EXPECT_EQ(document->getDocumentElement()->getTagName()->utf8(), "root");
    EXPECT_EQ(
        document->getDocumentElement()->getAttribute(
            ::jxx::NEW<::jxx::lang::String>("id"))->utf8(),
        "7");
}

TEST(XmlDomLsParserBasicStage, RejectsMissingInput) {
    const auto parser =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSParser(
                ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
                nullptr);
    EXPECT_THROW(parser->parse(nullptr), ::jxx::org::w3c::dom::ls::LSException);
}
