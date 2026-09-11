#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

TEST(XmlDomUserDataStage, StoresReplacesAndRemovesData) {
    const auto document =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder()
            ->newDocument();

    const auto element = document->createElement(
        ::jxx::NEW<::jxx::lang::String>("root"));
    const auto node = ::jxx::CAST<::jxx::org::w3c::dom::Node>(element);
    const auto key = ::jxx::NEW<::jxx::lang::String>("key");
    const auto first = ::jxx::NEW<::jxx::lang::String>("first");
    const auto second = ::jxx::NEW<::jxx::lang::String>("second");

    EXPECT_EQ(node->setUserData(key, first, nullptr), nullptr);
    EXPECT_EQ(
        ::jxx::CAST<::jxx::lang::String>(node->getUserData(key))->utf8(),
        "first");

    EXPECT_EQ(node->setUserData(key, second, nullptr), first);
    EXPECT_EQ(node->setUserData(key, nullptr, nullptr), second);
    EXPECT_EQ(node->getUserData(key), nullptr);
}

TEST(XmlDomUserDataStage, DataPersistsAcrossWrappersForSameNode) {
    const auto document =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder()
            ->newDocument();

    const auto root = document->createElement(
        ::jxx::NEW<::jxx::lang::String>("root"));
    document->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(root));

    const auto key = ::jxx::NEW<::jxx::lang::String>("persistent");
    const auto value = ::jxx::NEW<::jxx::lang::String>("value");
    root->setUserData(key, value, nullptr);

    const auto wrappedAgain = document->getDocumentElement();
    EXPECT_EQ(
        ::jxx::CAST<::jxx::lang::String>(
            wrappedAgain->getUserData(key))->utf8(),
        "value");
}
