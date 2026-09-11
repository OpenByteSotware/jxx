#include <gtest/gtest.h>

#include <vector>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.UserDataHandler.h"

namespace {

class RecordingUserDataHandler final
    : public ::jxx::lang::ClassBase<
          RecordingUserDataHandler,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::UserDataHandler> {
public:
    void handle(
        ::jxx::lang::jshort operation,
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& data,
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& source,
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& destination) override {
        operations.push_back(operation);
        lastKey = key;
        lastData = data;
        lastSource = source;
        lastDestination = destination;
    }

    std::vector<::jxx::lang::jshort> operations;
    ::jxx::Ptr<::jxx::lang::String> lastKey;
    ::jxx::Ptr<::jxx::lang::Object> lastData;
    ::jxx::Ptr<::jxx::org::w3c::dom::Node> lastSource;
    ::jxx::Ptr<::jxx::org::w3c::dom::Node> lastDestination;
};

::jxx::Ptr<::jxx::org::w3c::dom::Document> newDocument() {
    return ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
        ->newDocumentBuilder()
        ->newDocument();
}

} // namespace

TEST(XmlDomUserDataLifecycleStage, CloneAndRenameNotifyHandler) {
    const auto document = newDocument();
    const auto element = document->createElement(
        ::jxx::NEW<::jxx::lang::String>("root"));
    const auto node = ::jxx::CAST<::jxx::org::w3c::dom::Node>(element);
    const auto handler = ::jxx::NEW<RecordingUserDataHandler>();
    const auto key = ::jxx::NEW<::jxx::lang::String>("key");
    const auto data = ::jxx::NEW<::jxx::lang::String>("data");

    node->setUserData(key, data, handler);
    const auto clone = node->cloneNode(true);

    ASSERT_EQ(handler->operations.size(), 1U);
    EXPECT_EQ(
        handler->operations[0],
        ::jxx::org::w3c::dom::UserDataHandler::NODE_CLONED);
    EXPECT_EQ(handler->lastSource, node);
    EXPECT_EQ(handler->lastDestination, clone);

    document->renameNode(node, nullptr,
        ::jxx::NEW<::jxx::lang::String>("renamed"));

    ASSERT_EQ(handler->operations.size(), 2U);
    EXPECT_EQ(
        handler->operations[1],
        ::jxx::org::w3c::dom::UserDataHandler::NODE_RENAMED);
}

TEST(XmlDomUserDataLifecycleStage, ImportAndAdoptNotifyHandler) {
    const auto sourceDocument = newDocument();
    const auto targetDocument = newDocument();
    const auto element = sourceDocument->createElement(
        ::jxx::NEW<::jxx::lang::String>("item"));
    const auto node = ::jxx::CAST<::jxx::org::w3c::dom::Node>(element);
    const auto handler = ::jxx::NEW<RecordingUserDataHandler>();

    node->setUserData(
        ::jxx::NEW<::jxx::lang::String>("key"),
        ::jxx::NEW<::jxx::lang::String>("data"),
        handler);

    const auto imported = targetDocument->importNode(node, true);
    ASSERT_NE(imported, nullptr);
    ASSERT_EQ(handler->operations.size(), 1U);
    EXPECT_EQ(
        handler->operations[0],
        ::jxx::org::w3c::dom::UserDataHandler::NODE_IMPORTED);

    const auto adopted = targetDocument->adoptNode(node);
    ASSERT_NE(adopted, nullptr);
    ASSERT_EQ(handler->operations.size(), 2U);
    EXPECT_EQ(
        handler->operations[1],
        ::jxx::org::w3c::dom::UserDataHandler::NODE_ADOPTED);
}
