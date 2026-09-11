#include <gtest/gtest.h>

#include <type_traits>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSOutput.h"

TEST(XmlDomLsFoundationStage, InterfacesRemainAbstract) {
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::LSInput>);
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::LSOutput>);
    EXPECT_TRUE(std::is_abstract_v<::jxx::org::w3c::dom::ls::DOMImplementationLS>);
}

TEST(XmlDomLsFoundationStage, ConstantsMatchPublicContract) {
    using ::jxx::org::w3c::dom::ls::DOMImplementationLS;
    using ::jxx::org::w3c::dom::ls::LSException;

    EXPECT_EQ(1, DOMImplementationLS::MODE_SYNCHRONOUS);
    EXPECT_EQ(2, DOMImplementationLS::MODE_ASYNCHRONOUS);
    EXPECT_EQ(81, LSException::PARSE_ERR);
    EXPECT_EQ(82, LSException::SERIALIZE_ERR);
}

TEST(XmlDomLsFoundationStage, ExceptionPreservesCodeAndSupportsThrowing) {
    using ::jxx::org::w3c::dom::ls::LSException;
    const auto message = ::jxx::NEW<::jxx::lang::String>("parse stopped");
    const LSException exception(LSException::PARSE_ERR, message);

    EXPECT_EQ(LSException::PARSE_ERR, exception.code);
    EXPECT_THROW(throw exception, LSException);
}
