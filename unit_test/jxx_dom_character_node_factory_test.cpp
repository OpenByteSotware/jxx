#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.CDATASection.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Comment.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.ProcessingInstruction.h"

TEST(XmlDomCharacterFactoryStage, CreatesCommentCdataAndInstruction) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    const auto document = builder->newDocument();
    ASSERT_NE(document, nullptr);

    const auto comment = document->createComment(
        ::jxx::NEW<::jxx::lang::String>("note"));
    ASSERT_NE(comment, nullptr);
    EXPECT_EQ(comment->getNodeType(),
              ::jxx::org::w3c::dom::Node::COMMENT_NODE);
    EXPECT_EQ(comment->getData()->utf8(), "note");

    const auto cdata = document->createCDATASection(
        ::jxx::NEW<::jxx::lang::String>("<value>"));
    ASSERT_NE(cdata, nullptr);
    EXPECT_EQ(cdata->getNodeType(),
              ::jxx::org::w3c::dom::Node::CDATA_SECTION_NODE);
    EXPECT_EQ(cdata->getData()->utf8(), "<value>");

    const auto instruction = document->createProcessingInstruction(
        ::jxx::NEW<::jxx::lang::String>("target"),
        ::jxx::NEW<::jxx::lang::String>("value"));
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getNodeType(),
              ::jxx::org::w3c::dom::Node::PROCESSING_INSTRUCTION_NODE);
    EXPECT_EQ(instruction->getTarget()->utf8(), "target");
    EXPECT_EQ(instruction->getData()->utf8(), "value");
}
