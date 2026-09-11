#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMStringList.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

TEST(XmlDomLsConfigurationNameListContractStage, ContainsIsCaseInsensitive) {
    const auto names =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSSerializer()->getDomConfig()->getParameterNames();
    ASSERT_NE(names, nullptr);
    EXPECT_TRUE(names->contains(
        ::jxx::NEW<::jxx::lang::String>("COMMENTS")));
    EXPECT_TRUE(names->contains(
        ::jxx::NEW<::jxx::lang::String>("Xml-Declaration")));
}

TEST(XmlDomLsConfigurationNameListContractStage, ItemBoundsReturnNull) {
    const auto names =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSSerializer()->getDomConfig()->getParameterNames();
    ASSERT_NE(names, nullptr);
    EXPECT_EQ(names->item(-1), nullptr);
    EXPECT_EQ(names->item(names->getLength()), nullptr);
}

TEST(XmlDomLsConfigurationNameListContractStage, UnknownNameIsAbsent) {
    const auto names =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSSerializer()->getDomConfig()->getParameterNames();
    ASSERT_NE(names, nullptr);
    EXPECT_FALSE(names->contains(
        ::jxx::NEW<::jxx::lang::String>("unsupported")));
    EXPECT_FALSE(names->contains(nullptr));
}
