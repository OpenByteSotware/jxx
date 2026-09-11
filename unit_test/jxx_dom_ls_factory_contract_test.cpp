#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"

TEST(XmlDomLsFactoryContractStage, RejectsAsynchronousMode) {
    const auto factory = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    try {
        factory->createLSParser(
            ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_ASYNCHRONOUS,
            nullptr);
        FAIL() << "Expected DOMException";
    } catch (const ::jxx::org::w3c::dom::DOMException& exception) {
        EXPECT_EQ(exception.code, ::jxx::org::w3c::dom::DOMException::NOT_SUPPORTED_ERR);
    }
}

TEST(XmlDomLsFactoryContractStage, AcceptsStandardSchemaIdentifiers) {
    const auto factory = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    EXPECT_NE(factory->createLSParser(
        ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
        ::jxx::NEW<::jxx::lang::String>("http://www.w3.org/2001/XMLSchema")), nullptr);
    EXPECT_NE(factory->createLSParser(
        ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
        ::jxx::NEW<::jxx::lang::String>("http://www.w3.org/TR/REC-xml")), nullptr);
}

TEST(XmlDomLsFactoryContractStage, CoreFeatureReturnsLoadSaveImplementation) {
    const auto core = ::jxx::org::w3c::dom::internal::implementation();
    const auto feature = core->getFeature(
        ::jxx::NEW<::jxx::lang::String>("LS"),
        ::jxx::NEW<::jxx::lang::String>("3.0"));
    ASSERT_NE(feature, nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::org::w3c::dom::ls::DOMImplementationLS>(feature), nullptr);
}
