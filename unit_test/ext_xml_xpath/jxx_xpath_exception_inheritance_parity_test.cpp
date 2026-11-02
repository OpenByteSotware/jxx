#include <gtest/gtest.h>
#include <type_traits>

#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathException.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpressionException.h"
#include "lang/jxx.lang.Exception.h"

TEST(XPathExceptionInheritanceParity, XPathExceptionExtendsException) {
    static_assert(std::is_base_of_v<
        ::jxx::lang::Exception,
        ::jxx::ext::xml::xpath::XPathException>);
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Exception,
        ::jxx::ext::xml::xpath::XPathException>));
}

TEST(XPathExceptionInheritanceParity, XPathExpressionExceptionExtendsXPathException) {
    static_assert(std::is_base_of_v<
        ::jxx::ext::xml::xpath::XPathException,
        ::jxx::ext::xml::xpath::XPathExpressionException>);
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::xml::xpath::XPathException,
        ::jxx::ext::xml::xpath::XPathExpressionException>));
}

TEST(XPathExceptionInheritanceParity, PublicTypesHaveClassMetadata) {
    EXPECT_NE(nullptr, ::jxx::ext::xml::xpath::XPathException::Class());
    EXPECT_NE(nullptr, ::jxx::ext::xml::xpath::XPathExpressionException::Class());
}
