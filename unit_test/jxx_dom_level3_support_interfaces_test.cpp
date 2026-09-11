#include <gtest/gtest.h>

#include <type_traits>

#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMError.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMErrorHandler.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMLocator.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMStringList.h"
#include "org/w3c/dom/jxx.org.w3c.dom.UserDataHandler.h"

TEST(XmlDomLevel3SupportStage, ContractsRemainInterfaces) {
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::w3c::dom::DOMConfiguration>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::w3c::dom::DOMError>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::w3c::dom::DOMErrorHandler>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::w3c::dom::DOMLocator>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::w3c::dom::DOMStringList>));
    EXPECT_TRUE((std::is_abstract_v<::jxx::org::w3c::dom::UserDataHandler>));
}

TEST(XmlDomLevel3SupportStage, ConstantsMatchDomContracts) {
    EXPECT_EQ(::jxx::org::w3c::dom::DOMError::SEVERITY_WARNING, 1);
    EXPECT_EQ(::jxx::org::w3c::dom::DOMError::SEVERITY_FATAL_ERROR, 3);
    EXPECT_EQ(::jxx::org::w3c::dom::UserDataHandler::NODE_CLONED, 1);
    EXPECT_EQ(::jxx::org::w3c::dom::UserDataHandler::NODE_ADOPTED, 5);
}
