#include <gtest/gtest.h>

#include <type_traits>

#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationSource.h"

TEST(XmlDomImplementationDiscoveryStage, InterfacesRemainAbstract) {
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::DOMImplementationList>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::DOMImplementationSource>));
}

TEST(XmlDomImplementationDiscoveryStage, InterfacesDoNotInheritObject) {
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::DOMImplementationList>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::DOMImplementationSource>));
}
