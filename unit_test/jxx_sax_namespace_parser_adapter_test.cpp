#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.NamespaceSupport.h"

TEST(XmlSaxNamespaceSupportStage, ScopesPrefixMappings) {
    auto support = ::jxx::NEW<::jxx::org::xml::sax::helpers::NamespaceSupport>();
    support->pushContext();
    EXPECT_TRUE(support->declarePrefix(
        ::jxx::NEW<::jxx::lang::String>("p"),
        ::jxx::NEW<::jxx::lang::String>("urn:test")));
    auto parts = ::jxx::NEW<
        ::jxx::org::xml::sax::helpers::NamespaceSupport::StringArray>(3);
    auto result = support->processName(
        ::jxx::NEW<::jxx::lang::String>("p:item"), parts, false);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ((*result)[0]->utf8(), "urn:test");
    EXPECT_EQ((*result)[1]->utf8(), "item");
    support->popContext();
    EXPECT_EQ(support->getURI(::jxx::NEW<::jxx::lang::String>("p")), nullptr);
}
