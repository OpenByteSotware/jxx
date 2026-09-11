#include <gtest/gtest.h>

#include <type_traits>

#include "org/xml/sax/jxx.org.xml.sax.Attributes.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

TEST(XmlSax2InterfaceStage, InterfacesRemainAbstract) {
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::xml::sax::Attributes>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::xml::sax::ContentHandler>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::xml::sax::DTDHandler>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::xml::sax::XMLReader>));
}

TEST(XmlSax2InterfaceStage, InterfacesDoNotInheritObject) {
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::xml::sax::Attributes>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::xml::sax::ContentHandler>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::xml::sax::DTDHandler>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::xml::sax::XMLReader>));
}
