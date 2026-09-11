#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationSourceImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationSource.h"

TEST(XmlDomImplementationDiscoveryStage, FindsSupportedImplementation) {
    const auto source =
        ::jxx::org::w3c::dom::internal::implementationSource();

    const auto implementation = source->getDOMImplementation(
        ::jxx::NEW<::jxx::lang::String>("XML 3.0 Core 3.0"));

    ASSERT_NE(implementation, nullptr);
    EXPECT_TRUE(implementation->hasFeature(
        ::jxx::NEW<::jxx::lang::String>("XML"),
        ::jxx::NEW<::jxx::lang::String>("3.0")));
}

TEST(XmlDomImplementationDiscoveryStage, RejectsUnsupportedFeatures) {
    const auto source =
        ::jxx::org::w3c::dom::internal::implementationSource();

    EXPECT_EQ(
        source->getDOMImplementation(
            ::jxx::NEW<::jxx::lang::String>("Traversal 2.0")),
        nullptr);

    const auto values = source->getDOMImplementationList(
        ::jxx::NEW<::jxx::lang::String>("Traversal 2.0"));

    ASSERT_NE(values, nullptr);
    EXPECT_EQ(values->getLength(), 0);
    EXPECT_EQ(values->item(0), nullptr);
}

TEST(XmlDomImplementationDiscoveryStage, AcceptsPlusFeatureSyntax) {
    const auto source =
        ::jxx::org::w3c::dom::internal::implementationSource();

    const auto values = source->getDOMImplementationList(
        ::jxx::NEW<::jxx::lang::String>("+XML 3.0"));

    ASSERT_NE(values, nullptr);
    ASSERT_EQ(values->getLength(), 1);
    EXPECT_NE(values->item(0), nullptr);
}
