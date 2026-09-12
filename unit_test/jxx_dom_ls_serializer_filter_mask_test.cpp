#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializerFilter.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
namespace {

class ElementRejectFilter final
    : public ::jxx::lang::ClassBase<
          ElementRejectFilter,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::LSSerializerFilter> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ElementRejectFilter,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::LSSerializerFilter>;

    ::jxx::lang::jshort acceptNode(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) override {
        ++callbackCount;
        return node != nullptr && node->getNodeName()->utf8() == "remove"
            ? FILTER_REJECT
            : FILTER_ACCEPT;
    }

    ::jxx::lang::jint getWhatToShow() const override {
        return ::jxx::org::w3c::dom::traversal::NodeFilter::SHOW_ELEMENT;
    }

    ::jxx::lang::jint callbackCount = 0;
};

} // namespace

TEST(XmlDomLsSerializerFilterMaskStage, CallsFilterOnlyForVisibleNodeTypes) {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            ::jxx::NEW<::jxx::lang::String>("root"),
            nullptr);
    const auto root = document->getDocumentElement();
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(::jxx::NEW<::jxx::lang::String>("text"))));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createElement(::jxx::NEW<::jxx::lang::String>("remove"))));

    const auto filter = ::jxx::NEW<ElementRejectFilter>();
    const auto serializer =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSSerializer();
    serializer->setFilter(
        ::jxx::CAST<::jxx::org::w3c::dom::ls::LSSerializerFilter>(filter));

    const auto result = serializer->writeToString(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(root));
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->utf8(), "<root>text</root>");
    EXPECT_EQ(filter->callbackCount, 2);
}

TEST(XmlDomLsSerializerFilterMaskStage, ShowTextDoesNotFilterElements) {
    class RejectTextFilter final
        : public ::jxx::lang::ClassBase<
              RejectTextFilter,
              ::jxx::lang::Object,
              ::jxx::org::w3c::dom::ls::LSSerializerFilter> {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            RejectTextFilter,
            JxxSuper,
            ::jxx::org::w3c::dom::ls::LSSerializerFilter>;
        ::jxx::lang::jshort acceptNode(
            const ::jxx::Ptr<::jxx::org::w3c::dom::Node>&) override {
            return FILTER_REJECT;
        }
        ::jxx::lang::jint getWhatToShow() const override {
            return ::jxx::org::w3c::dom::traversal::NodeFilter::SHOW_TEXT;
        }
    };

    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            ::jxx::NEW<::jxx::lang::String>("root"),
            nullptr);
    document->getDocumentElement()->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(
            document->createTextNode(
                ::jxx::NEW<::jxx::lang::String>("hidden"))));
    const auto serializer =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSSerializer();
    serializer->setFilter(
        ::jxx::CAST<::jxx::org::w3c::dom::ls::LSSerializerFilter>(
            ::jxx::NEW<RejectTextFilter>()));

    EXPECT_EQ(serializer->writeToString(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(
            document->getDocumentElement()))->utf8(),
        "<root/>");
}
