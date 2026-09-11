#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSInputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSOutputImpl.h"

TEST(XmlDomLsValueObjectsStage, InputDefaultsAreEmpty) {
    const auto input = ::jxx::NEW<::jxx::org::w3c::dom::ls::internal::LSInputImpl>();
    EXPECT_EQ(nullptr, input->getCharacterStream());
    EXPECT_EQ(nullptr, input->getByteStream());
    EXPECT_EQ(nullptr, input->getStringData());
    EXPECT_FALSE(input->getCertifiedText());
}

TEST(XmlDomLsValueObjectsStage, InputRetainsProperties) {
    const auto input = ::jxx::NEW<::jxx::org::w3c::dom::ls::internal::LSInputImpl>();
    const auto data = ::jxx::NEW<::jxx::lang::String>("<root/>");
    const auto systemId = ::jxx::NEW<::jxx::lang::String>("memory:root");
    input->setStringData(data);
    input->setSystemId(systemId);
    input->setCertifiedText(true);
    EXPECT_EQ(data, input->getStringData());
    EXPECT_EQ(systemId, input->getSystemId());
    EXPECT_TRUE(input->getCertifiedText());
}

TEST(XmlDomLsValueObjectsStage, OutputDefaultsAndProperties) {
    const auto output = ::jxx::NEW<::jxx::org::w3c::dom::ls::internal::LSOutputImpl>();
    EXPECT_EQ(nullptr, output->getCharacterStream());
    EXPECT_EQ(nullptr, output->getByteStream());
    const auto encoding = ::jxx::NEW<::jxx::lang::String>("UTF-8");
    output->setEncoding(encoding);
    EXPECT_EQ(encoding, output->getEncoding());
}
