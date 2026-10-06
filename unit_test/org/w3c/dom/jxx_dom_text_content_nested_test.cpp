#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.CharacterData.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"

namespace {
::jxx::Ptr<::jxx::lang::String> text(const char* value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}
}

TEST(JxxDomTextContentNestedTest, ConcatenatesNestedTextInDocumentOrder) {
    const auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    ASSERT_NE(nullptr, factory);
    const auto builder = factory->newDocumentBuilder();
    ASSERT_NE(nullptr, builder);
    const auto document = builder->newDocument();
    ASSERT_NE(nullptr, document);

    const auto root = document->createElement(text("root"));
    const auto first = document->createElement(text("first"));
    const auto nested = document->createElement(text("nested"));
    const auto second = document->createElement(text("second"));

    first->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("alpha"))));
    nested->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("beta"))));
    first->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(nested));
    second->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("gamma"))));

    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(first));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(second));
    document->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(root));

    ASSERT_NE(nullptr, root->getTextContent());
    EXPECT_EQ("alphabetagamma", root->getTextContent()->utf8());
    ASSERT_NE(nullptr, first->getTextContent());
    EXPECT_EQ("alphabeta", first->getTextContent()->utf8());
}

TEST(JxxDomTextContentNestedTest, MutationReplacesAllChildrenWithOneTextNode) {
    const auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    const auto document = factory->newDocumentBuilder()->newDocument();
    const auto root = document->createElement(text("root"));
    const auto nested = document->createElement(text("nested"));

    nested->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("old-nested"))));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("old-prefix"))));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(nested));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("old-suffix"))));
    document->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(root));

    root->setTextContent(text("replacement"));

    ASSERT_NE(nullptr, root->getTextContent());
    EXPECT_EQ("replacement", root->getTextContent()->utf8());
    const auto children = root->getChildNodes();
    ASSERT_NE(nullptr, children);
    ASSERT_EQ(1, children->getLength());
    const auto child = children->item(0);
    ASSERT_NE(nullptr, child);
    EXPECT_EQ(::jxx::org::w3c::dom::Node::TEXT_NODE, child->getNodeType());
    ASSERT_NE(nullptr, child->getNodeValue());
    EXPECT_EQ("replacement", child->getNodeValue()->utf8());
}

TEST(JxxDomTextContentNestedTest, NullMutationRemovesAllChildren) {
    const auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    const auto document = factory->newDocumentBuilder()->newDocument();
    const auto root = document->createElement(text("root"));

    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("value"))));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createElement(text("nested"))));
    document->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(root));

    root->setTextContent(nullptr);

    ASSERT_NE(nullptr, root->getTextContent());
    EXPECT_TRUE(root->getTextContent()->utf8().empty());
    ASSERT_NE(nullptr, root->getChildNodes());
    EXPECT_EQ(0, root->getChildNodes()->getLength());
}
