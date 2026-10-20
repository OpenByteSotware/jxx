#include <gtest/gtest.h>

#include "ext/xml/bind/jxx.ext.xml.bind.ClassInitialization.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElement.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElementI.h"
#include "lang/jxx.lang.Object.h"
#include "jxx.initialize.h"

namespace
{
    TEST(JaxbElementClassInitialization, RegistersErasedObjectSpecialization)
    {
        ::jxx::initialize();
        
        const auto interfaceType =
            ::jxx::ext::xml::bind::JAXBElementI::JxxClassInfoMarker::Class();
        ASSERT_NE(interfaceType, nullptr);

        const auto elementType =
            ::jxx::ext::xml::bind::JAXBElement<
                ::jxx::lang::Object>::Class();
        ASSERT_NE(elementType, nullptr);
        ASSERT_NE(elementType->getName(), nullptr);
        EXPECT_FALSE(elementType->getName()->utf8().empty());
    }
}
