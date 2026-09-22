#include <gtest/gtest.h>

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"
#include "ext/xml/bind/jxx.ext.xml.bind.Marshaller.h"
#include "ext/xml/bind/jxx.ext.xml.bind.UnmarshalException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.Unmarshaller.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "generated_device_type_binding.h"

namespace {

::jxx::Ptr<::jxx::lang::Object> unmarshalText(const char* xml) {
    const auto context = ::jxx::test::generated::device::context();
    const auto unmarshaller = context->createUnmarshaller();
    const auto reader = ::jxx::NEW<::jxx::io::StringReader>(
        ::jxx::NEW<::jxx::lang::String>(xml));
    return unmarshaller->unmarshal(reader);
}

TEST(JaxbDeviceTypeRoundTripTest, UnmarshalsAllRequiredAttributes) {
    const auto value = unmarshalText(
        R"(<device id="12" config="7" hardware_scheme_filename="hardware.xml" security_scheme_filename="security.xml"/>)");

    EXPECT_EQ(12, ::jxx::test::generated::device::id(value));
    EXPECT_EQ(7, ::jxx::test::generated::device::config(value));
    EXPECT_EQ("hardware.xml", ::jxx::test::generated::device::hardwareFile(value)->utf8());
    EXPECT_EQ("security.xml", ::jxx::test::generated::device::securityFile(value)->utf8());
}

TEST(JaxbDeviceTypeRoundTripTest, MarshalsAndUnmarshalsEquivalentValues) {
    const auto context = ::jxx::test::generated::device::context();
    const auto original = ::jxx::test::generated::device::newDevice(
        12, 7,
        ::jxx::NEW<::jxx::lang::String>("hardware.xml"),
        ::jxx::NEW<::jxx::lang::String>("security.xml"));

    const auto xml = context->createMarshaller()->marshal(
        original,
        ::jxx::test::generated::device::marshallingDescriptor());

    EXPECT_NE(std::string::npos, xml->utf8().find("id=\"12\""));
    EXPECT_NE(std::string::npos, xml->utf8().find("config=\"7\""));
    EXPECT_NE(std::string::npos, xml->utf8().find("hardware_scheme_filename=\"hardware.xml\""));
    EXPECT_NE(std::string::npos, xml->utf8().find("security_scheme_filename=\"security.xml\""));

    const auto reader = ::jxx::NEW<::jxx::io::StringReader>(xml);
    const auto restored = context->createUnmarshaller()->unmarshal(reader);

    EXPECT_EQ(12, ::jxx::test::generated::device::id(restored));
    EXPECT_EQ(7, ::jxx::test::generated::device::config(restored));
    EXPECT_EQ("hardware.xml", ::jxx::test::generated::device::hardwareFile(restored)->utf8());
    EXPECT_EQ("security.xml", ::jxx::test::generated::device::securityFile(restored)->utf8());
}

TEST(JaxbDeviceTypeRoundTripTest, EscapesAttributeText) {
    const auto context = ::jxx::test::generated::device::context();
    const auto value = ::jxx::test::generated::device::newDevice(
        1, 2,
        ::jxx::NEW<::jxx::lang::String>("hardware&v2.xml"),
        ::jxx::NEW<::jxx::lang::String>("security\"v2.xml"));

    const auto xml = context->createMarshaller()->marshal(
        value,
        ::jxx::test::generated::device::marshallingDescriptor());

    EXPECT_NE(std::string::npos, xml->utf8().find("hardware&amp;v2.xml"));
    EXPECT_NE(std::string::npos, xml->utf8().find("security&quot;v2.xml"));
}

TEST(JaxbDeviceTypeRoundTripTest, RejectsMissingRequiredAttribute) {
    EXPECT_THROW(
        unmarshalText(
            R"(<device id="12" config="7" hardware_scheme_filename="hardware.xml"/>)"),
        ::jxx::ext::xml::bind::UnmarshalException);
}

TEST(JaxbDeviceTypeRoundTripTest, RejectsInvalidRestrictedValues) {
    EXPECT_THROW(
        unmarshalText(
            R"(<device id="70000" config="7" hardware_scheme_filename="hardware.xml" security_scheme_filename="security.xml"/>)"),
        ::jxx::lang::IllegalArgumentException);

    EXPECT_THROW(
        unmarshalText(
            R"(<device id="12" config="256" hardware_scheme_filename="hardware.xml" security_scheme_filename="security.xml"/>)"),
        ::jxx::lang::IllegalArgumentException);

    EXPECT_THROW(
        unmarshalText(
            R"(<device id="12" config="7" hardware_scheme_filename="folder/hardware.xml" security_scheme_filename="security.xml"/>)"),
        ::jxx::lang::IllegalArgumentException);
}

} // namespace
