#include <gtest/gtest.h>
#include <type_traits>

#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementDecl.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlRegistry.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElement.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "lang/jxx.lang.String.h"

namespace {

using ::jxx::ext::xml::bind::JAXBElement;
using ::jxx::ext::xml::bind::annotation::XmlElementDecl;
using ::jxx::ext::xml::bind::annotation::XmlRegistry;
using ::jxx::ext::xml::namespace_::QName;

static_assert(std::is_base_of_v<::jxx::lang::Object, QName>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlElementDecl>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlRegistry>);
static_assert(std::is_base_of_v<
    ::jxx::lang::Object,
    JAXBElement<::jxx::lang::String>>);

TEST(JaxbGeneratedSourceCompatibilityTest, PublicTypesAreAvailable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, QName>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, XmlElementDecl>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, XmlRegistry>));
}

} // namespace
