#include <gtest/gtest.h>
#include <type_traits>

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.UnmarshalException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.Unmarshaller.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.DomUnmarshaller.h"

namespace {

using ::jxx::ext::xml::bind::JAXBContext;
using ::jxx::ext::xml::bind::JAXBException;
using ::jxx::ext::xml::bind::UnmarshalException;
using ::jxx::ext::xml::bind::Unmarshaller;
using ::jxx::ext::xml::bind::internal::DomUnmarshaller;

static_assert(std::is_base_of_v<::jxx::lang::Object, JAXBContext>);
static_assert(std::is_base_of_v<::jxx::lang::Exception, JAXBException>);
static_assert(std::is_base_of_v<JAXBException, UnmarshalException>);
static_assert(std::is_abstract_v<Unmarshaller>);
static_assert(std::is_base_of_v<Unmarshaller, DomUnmarshaller>);

TEST(JaxbRuntimeTypeTest, PublicTypeRelationshipsAreStable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, JAXBContext>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Exception, JAXBException>));
    EXPECT_TRUE((std::is_base_of_v<JAXBException, UnmarshalException>));
    EXPECT_TRUE((std::is_abstract_v<Unmarshaller>));
    EXPECT_TRUE((std::is_base_of_v<Unmarshaller, DomUnmarshaller>));
}

} // namespace
