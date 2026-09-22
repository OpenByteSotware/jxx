#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElement.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElementI.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ElementWrapperFactory.h"
#include "lang/jxx.lang.String.h"
namespace {
using ::jxx::ext::xml::bind::JAXBElement;
using ::jxx::ext::xml::bind::JAXBElementI;
using ::jxx::ext::xml::bind::metadata::ElementWrapperFactory;
static_assert(std::is_abstract_v<JAXBElementI>);
static_assert(std::is_abstract_v<ElementWrapperFactory>);
static_assert(std::is_base_of_v<JAXBElementI, JAXBElement<::jxx::lang::String>>);
TEST(JaxbElementIntegrationTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<JAXBElementI>));
    EXPECT_TRUE((std::is_abstract_v<ElementWrapperFactory>));
    EXPECT_TRUE((std::is_base_of_v<JAXBElementI, JAXBElement<::jxx::lang::String>>));
}
}
