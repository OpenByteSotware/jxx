#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSOutput.h"
TEST(XmlDomLsFactoryStage, ReturnsStableFactory) {
    const auto first = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto second = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first, second);
}

TEST(XmlDomLsFactoryStage, CreatesIndependentInputDescriptors) {
    const auto factory = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto first = factory->createLSInput();
    const auto second = factory->createLSInput();
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first, second);
    first->setStringData(::jxx::NEW<::jxx::lang::String>("<root/>"));
    EXPECT_NE(first->getStringData(), nullptr);
    EXPECT_EQ(second->getStringData(), nullptr);
}

TEST(XmlDomLsFactoryStage, CreatesIndependentOutputDescriptors) {
    const auto factory = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto first = factory->createLSOutput();
    const auto second = factory->createLSOutput();
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first, second);
    first->setEncoding(::jxx::NEW<::jxx::lang::String>("UTF-8"));
    EXPECT_NE(first->getEncoding(), nullptr);
    EXPECT_EQ(second->getEncoding(), nullptr);
}

TEST(XmlDomLsFactoryStage, ImplementedFactoriesAreAvailable) {
    const auto factory = ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    EXPECT_NE(factory->createLSSerializer(), nullptr);
    EXPECT_NE(factory->createLSParser(
        ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
        nullptr), nullptr);
}
