#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMStringList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"

TEST(XmlDomConfigurationStage, ExposesAndUpdatesRequiredBooleanParameters) {
    const auto document =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder()
            ->newDocument();
    const auto configuration = document->getDomConfig();
    ASSERT_NE(configuration, nullptr);

    const auto comments = ::jxx::NEW<::jxx::lang::String>("COMMENTS");
    EXPECT_TRUE(configuration->canSetParameter(
        comments,
        ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::FALSE)));

    configuration->setParameter(
        comments,
        ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::FALSE));

    EXPECT_FALSE(
        ::jxx::CAST<::jxx::lang::Boolean>(
            configuration->getParameter(comments))->booleanValue());

    const auto names = configuration->getParameterNames();
    ASSERT_NE(names, nullptr);
    EXPECT_TRUE(names->contains(
        ::jxx::NEW<::jxx::lang::String>("comments")));
}

TEST(XmlDomConfigurationStage, RejectsUnsupportedAndUnknownValues) {
    const auto configuration =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder()
            ->newDocument()
            ->getDomConfig();

    EXPECT_FALSE(configuration->canSetParameter(
        ::jxx::NEW<::jxx::lang::String>("validate"),
        ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::TRUE)));

    EXPECT_THROW(
        configuration->getParameter(
            ::jxx::NEW<::jxx::lang::String>("unknown")),
        ::jxx::org::w3c::dom::DOMException);
}
