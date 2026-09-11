#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"

namespace jxx::org::w3c::dom::ls::internal {

class LSParserImpl final
    : public ::jxx::lang::ClassBase<
          LSParserImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::LSParser> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        LSParserImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::LSParser>;

    LSParserImpl();
    ~LSParserImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>
    getDomConfig() const override;

    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>
    getFilter() const override;

    void setFilter(
        const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter>& filter) override;

    ::jxx::lang::jbool getAsync() const override;
    ::jxx::lang::jbool getBusy() const override;

    ::jxx::Ptr<::jxx::org::w3c::dom::Document> parse(
        const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input) override;

    ::jxx::Ptr<::jxx::org::w3c::dom::Document> parseURI(
        const ::jxx::Ptr<::jxx::lang::String>& uri) override;

    ::jxx::Ptr<::jxx::org::w3c::dom::Node> parseWithContext(
        const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input,
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& contextArg,
        ::jxx::lang::jshort action) override;

    void abort() override;

private:
    ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration> domConfig_;
    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParserFilter> filter_;
    ::jxx::lang::jbool busy_;
    ::jxx::lang::jbool abortRequested_;
};

} // namespace jxx::org::w3c::dom::ls::internal
