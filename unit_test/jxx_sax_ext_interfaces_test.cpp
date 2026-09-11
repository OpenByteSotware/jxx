#include <gtest/gtest.h>

#include <type_traits>

#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DeclHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DefaultHandler2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.EntityResolver2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.LexicalHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2.h"

TEST(XmlSaxExtStage, ExtensionContractsRemainInterfaces) {
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::xml::sax::ext::DeclHandler>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::xml::sax::ext::EntityResolver2>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::xml::sax::ext::LexicalHandler>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::xml::sax::ext::Locator2>));
}

TEST(XmlSaxExtStage, DefaultHandler2IsConcrete) {
    EXPECT_FALSE((std::is_abstract_v<
        ::jxx::org::xml::sax::ext::DefaultHandler2>));

    const auto handler =
        ::jxx::NEW<::jxx::org::xml::sax::ext::DefaultHandler2>();

    EXPECT_EQ(handler->getExternalSubset(nullptr, nullptr), nullptr);
    EXPECT_EQ(handler->resolveEntity(nullptr, nullptr), nullptr);
}
