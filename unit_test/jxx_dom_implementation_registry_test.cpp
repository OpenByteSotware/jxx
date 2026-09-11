#include <gtest/gtest.h>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/bootstrap/jxx.org.w3c.dom.bootstrap.DOMImplementationRegistry.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationList.h"

TEST(XmlDomImplementationRegistryStage, FindsDefaultImplementation) {
    const auto registry =
        ::jxx::org::w3c::dom::bootstrap::
            DOMImplementationRegistry::newInstance();

    ASSERT_NE(registry, nullptr);

    const auto implementation =
        registry->getDOMImplementation(
            ::jxx::NEW<::jxx::lang::String>(
                "XML 3.0 Core 3.0"));

    ASSERT_NE(implementation, nullptr);
    EXPECT_TRUE(
        implementation->hasFeature(
            ::jxx::NEW<::jxx::lang::String>("XML"),
            ::jxx::NEW<::jxx::lang::String>("3.0")));
}

TEST(XmlDomImplementationRegistryStage, AggregatesMatchingImplementations) {
    const auto registry =
        ::jxx::org::w3c::dom::bootstrap::
            DOMImplementationRegistry::newInstance();

    const auto values =
        registry->getDOMImplementationList(
            ::jxx::NEW<::jxx::lang::String>("Core 3.0"));

    ASSERT_NE(values, nullptr);
    ASSERT_EQ(values->getLength(), 1);
    EXPECT_NE(values->item(0), nullptr);
    EXPECT_EQ(values->item(1), nullptr);
}

TEST(XmlDomImplementationRegistryStage, RejectsNullSource) {
    const auto registry =
        ::jxx::org::w3c::dom::bootstrap::
            DOMImplementationRegistry::newInstance();

    EXPECT_THROW(
        registry->addSource(nullptr),
        ::jxx::lang::NullPointerException);
}

TEST(XmlDomImplementationRegistryStage, ExposesPropertyName) {
    EXPECT_EQ(
        ::jxx::org::w3c::dom::bootstrap::
            DOMImplementationRegistry::PROPERTY->utf8(),
        "org.w3c.dom.DOMImplementationSourceList");
}
