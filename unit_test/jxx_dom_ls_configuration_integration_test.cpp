#include <gtest/gtest.h>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMStringList.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

TEST(XmlDomLsConfigurationIntegrationStage, ParserConfigurationIsStable) {
    const auto parser = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
        ->createLSParser(::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS, nullptr);
    const auto first = parser->getDomConfig();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first, parser->getDomConfig());
    EXPECT_TRUE(first->getParameterNames()->contains(
        ::jxx::NEW<::jxx::lang::String>("namespaces")));
}

TEST(XmlDomLsConfigurationIntegrationStage, SerializerRetainsParameterValue) {
    const auto config = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
        ->createLSSerializer()->getDomConfig();
    ASSERT_NE(config, nullptr);
    const auto value = ::jxx::lang::Boolean::valueOf(true);
    const auto objectValue = ::jxx::CAST<::jxx::lang::Object>(value);
    config->setParameter(::jxx::NEW<::jxx::lang::String>("format-pretty-print"), objectValue);
    EXPECT_EQ(config->getParameter(
        ::jxx::NEW<::jxx::lang::String>("format-pretty-print")), objectValue);
}

TEST(XmlDomLsConfigurationIntegrationStage, OwnersHaveIndependentConfiguration) {
    const auto factory = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    EXPECT_NE(factory->createLSSerializer()->getDomConfig(),
        factory->createLSSerializer()->getDomConfig());
}

TEST(XmlDomLsConfigurationIntegrationStage, UnsupportedParameterIsRejected) {
    const auto config = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
        ->createLSSerializer()->getDomConfig();
    EXPECT_FALSE(config->canSetParameter(
        ::jxx::NEW<::jxx::lang::String>("unsupported"), nullptr));
    EXPECT_THROW(config->getParameter(
        ::jxx::NEW<::jxx::lang::String>("unsupported")),
        ::jxx::org::w3c::dom::DOMException);
}
