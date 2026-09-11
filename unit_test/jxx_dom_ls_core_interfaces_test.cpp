#include <gtest/gtest.h>

#include <type_traits>

#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParserFilter.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSResourceResolver.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializerFilter.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"

TEST(XmlDomLsCoreInterfacesStage, ContractsRemainAbstract) {
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::LSParser>);
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::LSParserFilter>);
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::LSResourceResolver>);
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::LSSerializer>);
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::LSSerializerFilter>);
}

TEST(XmlDomLsCoreInterfacesStage, ParserConstantsMatchContract) {
    using P = ::jxx::org::w3c::dom::ls::LSParser;
    EXPECT_EQ(1, P::ACTION_APPEND_AS_CHILDREN);
    EXPECT_EQ(2, P::ACTION_REPLACE_CHILDREN);
    EXPECT_EQ(3, P::ACTION_INSERT_BEFORE);
    EXPECT_EQ(4, P::ACTION_INSERT_AFTER);
    EXPECT_EQ(5, P::ACTION_REPLACE);
}

TEST(XmlDomLsCoreInterfacesStage, FilterConstantsMatchContract) {
    using F = ::jxx::org::w3c::dom::ls::LSParserFilter;
    using N = ::jxx::org::w3c::dom::traversal::NodeFilter;
    EXPECT_EQ(1, F::FILTER_ACCEPT);
    EXPECT_EQ(4, F::FILTER_INTERRUPT);
    EXPECT_EQ(-1, N::SHOW_ALL);
    EXPECT_EQ(0x00000800, N::SHOW_NOTATION);
}

TEST(XmlDomLsCoreInterfacesStage, SerializerFilterExtendsNodeFilter) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::org::w3c::dom::traversal::NodeFilter,
        ::jxx::org::w3c::dom::ls::LSSerializerFilter>));
}
