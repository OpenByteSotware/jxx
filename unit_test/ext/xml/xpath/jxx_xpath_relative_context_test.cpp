#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

namespace {

::jxx::Ptr<::jxx::lang::String> text(const char* value)
{
    return ::jxx::NEW<::jxx::lang::String>(value);
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> parse(const char* source)
{
    const auto factory =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    factory->setNamespaceAware(true);
    const auto builder = factory->newDocumentBuilder();
    const auto input = ::jxx::NEW<::jxx::org::xml::sax::InputSource>(
        ::jxx::CAST<::jxx::io::Reader>(
            ::jxx::NEW<::jxx::io::StringReader>(text(source))));
    return builder->parse(input);
}

::jxx::Ptr<::jxx::org::w3c::dom::NodeList> evaluateNodes(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& context,
    const char* expression)
{
    const auto xpath =
        ::jxx::ext::xml::xpath::XPathFactory::newInstance()->newXPath();
    const auto compiled = xpath->compile(text(expression));
    return ::jxx::CAST<::jxx::org::w3c::dom::NodeList>(
        compiled->evaluate(
            ::jxx::CAST<::jxx::lang::Object>(context),
            ::jxx::ext::xml::xpath::XPathConstants::NODESET()));
}

TEST(XPathRelativeContextTest, SelectsDirectChildrenAfterDotStep)
{
    const auto document = parse(
        "<Layout><OtisPhysicalButton id=\"1\"/>"
        "<Other/><OtisPhysicalButton id=\"2\"/></Layout>");
    const auto nodes = evaluateNodes(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(
            document->getDocumentElement()),
        "./OtisPhysicalButton");

    ASSERT_NE(nodes, nullptr);
    ASSERT_EQ(nodes->getLength(), 2);
    ASSERT_NE(nodes->item(0), nullptr);
    ASSERT_NE(nodes->item(1), nullptr);
    EXPECT_EQ(nodes->item(0)->getNodeName()->utf8(), "OtisPhysicalButton");
    EXPECT_EQ(nodes->item(1)->getNodeName()->utf8(), "OtisPhysicalButton");
}

TEST(XPathRelativeContextTest, DotSelectsTheContextNode)
{
    const auto document = parse("<Layout><Child/></Layout>");
    const auto context = ::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->getDocumentElement());
    const auto nodes = evaluateNodes(context, ".");

    ASSERT_NE(nodes, nullptr);
    ASSERT_EQ(nodes->getLength(), 1);
    EXPECT_EQ(nodes->item(0), context);
}

TEST(XPathRelativeContextTest, SelectsAllDirectElementChildrenAfterDotStep)
{
    const auto document = parse("<Layout><First/><Second/></Layout>");
    const auto nodes = evaluateNodes(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(
            document->getDocumentElement()),
        "./*");

    ASSERT_NE(nodes, nullptr);
    ASSERT_EQ(nodes->getLength(), 2);
    EXPECT_EQ(nodes->item(0)->getNodeName()->utf8(), "First");
    EXPECT_EQ(nodes->item(1)->getNodeName()->utf8(), "Second");
}

TEST(XPathRelativeContextTest, SelectsNestedRelativePathAfterDotStep)
{
    const auto document = parse(
        "<Layout><Panel><Label/><Other/></Panel><Panel><Label/></Panel></Layout>");
    const auto nodes = evaluateNodes(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(
            document->getDocumentElement()),
        "./Panel/Label");

    ASSERT_NE(nodes, nullptr);
    ASSERT_EQ(nodes->getLength(), 2);
    EXPECT_EQ(nodes->item(0)->getNodeName()->utf8(), "Label");
    EXPECT_EQ(nodes->item(1)->getNodeName()->utf8(), "Label");
}

} // namespace
