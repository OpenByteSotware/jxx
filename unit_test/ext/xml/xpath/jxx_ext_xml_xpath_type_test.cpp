#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPath.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpression.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"
namespace {
using namespace ::jxx::ext::xml::xpath;
static_assert(std::is_abstract_v<XPath>);
static_assert(std::is_abstract_v<XPathExpression>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XPathFactory>);
TEST(XPathTypeTest, PublicRelationshipsAreStable) {
 EXPECT_TRUE((std::is_abstract_v<XPath>));
 EXPECT_TRUE((std::is_abstract_v<XPathExpression>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, XPathFactory>));
}
} // namespace
