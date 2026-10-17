#include <gtest/gtest.h>

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

TEST(JAXBContextClassArrayParity, RejectsEmptyBoundClassArray) {
    auto classes = ::jxx::NEW<::jxx::ext::xml::bind::JAXBContext::ClassArray>(0);
    EXPECT_THROW(
        ::jxx::ext::xml::bind::JAXBContext::newInstance(classes),
        ::jxx::lang::IllegalArgumentException);
}

TEST(JAXBContextClassArrayParity, RejectsUnregisteredBoundClass) {
    auto classes = ::jxx::NEW<::jxx::ext::xml::bind::JAXBContext::ClassArray>(1);
    (*classes)[0] = ::jxx::lang::String::Class();
    EXPECT_THROW(
        ::jxx::ext::xml::bind::JAXBContext::newInstance(classes),
        ::jxx::ext::xml::bind::JAXBException);
}
