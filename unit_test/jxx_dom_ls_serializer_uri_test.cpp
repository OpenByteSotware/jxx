#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSOutput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

namespace {

::jxx::Ptr<::jxx::org::w3c::dom::Node> createDocumentNode() {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            ::jxx::NEW<::jxx::lang::String>("root"),
            nullptr);
    return ::jxx::CAST<::jxx::org::w3c::dom::Node>(document);
}

std::string readFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    std::ostringstream content;
    content << input.rdbuf();
    return content.str();
}

} // namespace

TEST(XmlDomLsSerializerUriStage, WritesDirectlyToPath) {
    const std::string path = "jxx_dom_ls_serializer_uri_test.xml";
    std::remove(path.c_str());
    const auto serializer =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSSerializer();

    EXPECT_TRUE(serializer->writeToURI(
        createDocumentNode(),
        ::jxx::NEW<::jxx::lang::String>(path.c_str())));
    EXPECT_EQ(readFile(path), "<root/>");
    std::remove(path.c_str());
}

TEST(XmlDomLsSerializerUriStage, RoutesOutputSystemIdToUriWriter) {
    const std::string path = "jxx_dom_ls_serializer_system_id_test.xml";
    std::remove(path.c_str());
    const auto factory =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation();
    const auto output = factory->createLSOutput();
    output->setSystemId(
        ::jxx::NEW<::jxx::lang::String>(path.c_str()));

    EXPECT_TRUE(factory->createLSSerializer()->write(
        createDocumentNode(), output));
    EXPECT_EQ(readFile(path), "<root/>");
    std::remove(path.c_str());
}

TEST(XmlDomLsSerializerUriStage, RejectsUnsupportedScheme) {
    const auto serializer =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSSerializer();
    EXPECT_THROW(
        serializer->writeToURI(
            createDocumentNode(),
            ::jxx::NEW<::jxx::lang::String>("https://example.invalid/output.xml")),
        ::jxx::org::w3c::dom::ls::LSException);
}
