#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSOutput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

TEST(XmlDomLsSerializerOutputStage, WritesUtf8ToByteStream) {
    const auto core = ::jxx::org::w3c::dom::internal::implementation();
    const auto document = core->createDocument(
        nullptr,
        ::jxx::NEW<::jxx::lang::String>("root"),
        nullptr);
    const auto factory =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto output = factory->createLSOutput();
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    output->setByteStream(
        ::jxx::CAST<::jxx::io::OutputStream>(bytes));

    const auto serializer = factory->createLSSerializer();
    EXPECT_TRUE(serializer->write(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(document),
        output));
    EXPECT_EQ(bytes->toString()->utf8(), "<?xml version=\"1.0\" encoding=\"UTF-8\"?><root/>");
}

TEST(XmlDomLsSerializerOutputStage, RejectsMissingDestination) {
    const auto factory =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto serializer = factory->createLSSerializer();
    EXPECT_THROW(
        serializer->write(nullptr, nullptr),
        ::jxx::org::w3c::dom::ls::LSException);
}

TEST(XmlDomLsSerializerOutputStage, RejectsDescriptorWithoutDestination) {
    const auto core = ::jxx::org::w3c::dom::internal::implementation();
    const auto document = core->createDocument(
        nullptr,
        ::jxx::NEW<::jxx::lang::String>("root"),
        nullptr);
    const auto factory =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto serializer = factory->createLSSerializer();
    EXPECT_THROW(
        serializer->write(
            ::jxx::CAST<::jxx::org::w3c::dom::Node>(document),
            factory->createLSOutput()),
        ::jxx::org::w3c::dom::ls::LSException);
}
