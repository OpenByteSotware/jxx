#include <gtest/gtest.h>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

TEST(XmlDomLsConfigurationContractStage, ProvidesBooleanDefaults) {
    const auto config = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
        ->createLSSerializer()->getDomConfig();
    const auto comments = ::jxx::CAST<::jxx::lang::Boolean>(
        config->getParameter(::jxx::NEW<::jxx::lang::String>("comments")));
    const auto pretty = ::jxx::CAST<::jxx::lang::Boolean>(
        config->getParameter(::jxx::NEW<::jxx::lang::String>("format-pretty-print")));
    ASSERT_NE(comments, nullptr);
    ASSERT_NE(pretty, nullptr);
    EXPECT_TRUE(comments->booleanValue());
    EXPECT_FALSE(pretty->booleanValue());
}

TEST(XmlDomLsConfigurationContractStage, NamesAreCaseInsensitive) {
    const auto config = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
        ->createLSSerializer()->getDomConfig();
    config->setParameter(::jxx::NEW<::jxx::lang::String>("COMMENTS"),
        ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(false)));
    const auto result = ::jxx::CAST<::jxx::lang::Boolean>(
        config->getParameter(::jxx::NEW<::jxx::lang::String>("comments")));
    ASSERT_NE(result, nullptr);
    EXPECT_FALSE(result->booleanValue());
}

TEST(XmlDomLsConfigurationContractStage, RejectsWrongValueType) {
    const auto config = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
        ->createLSSerializer()->getDomConfig();
    EXPECT_FALSE(config->canSetParameter(
        ::jxx::NEW<::jxx::lang::String>("comments"),
        ::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<::jxx::lang::String>("false"))));
    try {
        config->setParameter(
            ::jxx::NEW<::jxx::lang::String>("comments"),
            ::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<::jxx::lang::String>("false")));
        FAIL() << "Expected DOMException";
    } catch (const ::jxx::org::w3c::dom::DOMException& exception) {
        EXPECT_EQ(exception.code, ::jxx::org::w3c::dom::DOMException::TYPE_MISMATCH_ERR);
    }
}
