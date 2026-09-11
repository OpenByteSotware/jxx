#include <gtest/gtest.h>

#include <type_traits>

#include "org/w3c/dom/jxx.org.w3c.dom.Entity.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NameList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Notation.h"

TEST(
    XmlDomRemainingInterfacesStage,
    InterfacesRemainAbstract) {
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::Entity>));

    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::Notation>));

    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::NameList>));
}

TEST(
    XmlDomRemainingInterfacesStage,
    NodeInterfacesDoNotInheritObjectDirectly) {
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::Entity>));

    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::Notation>));

    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::NameList>));
}
