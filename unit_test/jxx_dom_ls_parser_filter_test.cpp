#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParserFilter.h"
#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
namespace {

class RejectNamedElementFilter final
    : public ::jxx::lang::ClassBase<
          RejectNamedElementFilter,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::LSParserFilter> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        RejectNamedElementFilter,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::LSParserFilter>;

    explicit RejectNamedElementFilter(const char* rejected)
        : rejected_(rejected) {
    }

    ::jxx::lang::jshort startElement(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Element>& element) override {
        if (element != nullptr && element->getTagName()->utf8() == rejected_) {
            return FILTER_REJECT;
        }
        return FILTER_ACCEPT;
    }

    ::jxx::lang::jshort acceptNode(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>&) override {
        return FILTER_ACCEPT;
    }

    ::jxx::lang::jint getWhatToShow() const override {
        return ::jxx::org::w3c::dom::traversal::NodeFilter::SHOW_ELEMENT;
    }

private:
    const char* rejected_;
};

class InterruptFilter final
    : public ::jxx::lang::ClassBase<
          InterruptFilter,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::LSParserFilter> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        InterruptFilter,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::LSParserFilter>;

    ::jxx::lang::jshort startElement(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Element>&) override {
        return FILTER_INTERRUPT;
    }
    ::jxx::lang::jshort acceptNode(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>&) override {
        return FILTER_ACCEPT;
    }
    ::jxx::lang::jint getWhatToShow() const override {
        return ::jxx::org::w3c::dom::traversal::NodeFilter::SHOW_ELEMENT;
    }
};

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParser> parserWith(
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>& filter) {
    const auto parser =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSParser(
                ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
                nullptr);
    parser->setFilter(filter);
    return parser;
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput> input(const char* text) {
    const auto value =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSInput();
    value->setStringData(::jxx::NEW<::jxx::lang::String>(text));
    return value;
}

} // namespace

TEST(XmlDomLsParserFilterStage, RejectRemovesElementSubtree) {
    const auto filter = ::jxx::NEW<RejectNamedElementFilter>("remove");
    const auto document = parserWith(
        ::jxx::CAST<::jxx::org::w3c::dom::ls::LSParserFilter>(filter))
            ->parse(input("<root><keep/><remove><nested/></remove></root>"));

    EXPECT_EQ(document->getElementsByTagName(
        ::jxx::NEW<::jxx::lang::String>("keep"))->getLength(), 1);
    EXPECT_EQ(document->getElementsByTagName(
        ::jxx::NEW<::jxx::lang::String>("remove"))->getLength(), 0);
    EXPECT_EQ(document->getElementsByTagName(
        ::jxx::NEW<::jxx::lang::String>("nested"))->getLength(), 0);
}

TEST(XmlDomLsParserFilterStage, InterruptRaisesParseException) {
    const auto filter = ::jxx::NEW<InterruptFilter>();
    EXPECT_THROW(
        parserWith(::jxx::CAST<::jxx::org::w3c::dom::ls::LSParserFilter>(filter))
            ->parse(input("<root/>")),
        ::jxx::org::w3c::dom::ls::LSException);
}
