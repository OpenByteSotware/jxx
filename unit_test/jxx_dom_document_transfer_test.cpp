#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

TEST(XmlDomDocumentTransfer, ImportsAndRenamesNodes) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    const auto sourceDocument = builder->newDocument();
    const auto sourceElement = sourceDocument->createElement(
        ::jxx::NEW<::jxx::lang::String>("source"));
    sourceDocument->appendChild(sourceElement);

    const auto targetDocument = builder->newDocument();
    const auto imported = targetDocument->importNode(sourceElement, true);
    ASSERT_NE(imported, nullptr);
    EXPECT_EQ(imported->getNodeName()->utf8(), "source");
    EXPECT_NE(imported->getOwnerDocument(), sourceDocument);

    const auto renamed = targetDocument->renameNode(
        imported,
        ::jxx::NEW<::jxx::lang::String>("urn:test"),
        ::jxx::NEW<::jxx::lang::String>("p:renamed"));
    ASSERT_NE(renamed, nullptr);
    EXPECT_EQ(renamed->getNodeName()->utf8(), "p:renamed");
}

TEST(XmlDomDocumentTransfer, AdoptMovesAcrossDocuments) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    const auto sourceDocument = builder->newDocument();
    const auto sourceElement = sourceDocument->createElement(
        ::jxx::NEW<::jxx::lang::String>("moving"));
    sourceDocument->appendChild(sourceElement);

    const auto targetDocument = builder->newDocument();
    const auto adopted = targetDocument->adoptNode(sourceElement);
    ASSERT_NE(adopted, nullptr);
    EXPECT_EQ(adopted->getNodeName()->utf8(), "moving");
    EXPECT_EQ(sourceDocument->getDocumentElement(), nullptr);
}
