#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

namespace jxx::org::w3c::dom::ls::internal {

class LSSerializerImpl final
    : public ::jxx::lang::ClassBase<
          LSSerializerImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::LSSerializer> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        LSSerializerImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::LSSerializer>;

    LSSerializerImpl();
    ~LSSerializerImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration> getDomConfig() const override;
    ::jxx::Ptr<::jxx::lang::String> getNewLine() const override;
    void setNewLine(const ::jxx::Ptr<::jxx::lang::String>& newLine) override;
    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializerFilter> getFilter() const override;
    void setFilter(const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializerFilter>& filter) override;

    ::jxx::lang::jbool write(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSOutput>& destination) override;

    ::jxx::lang::jbool writeToURI(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg,
        const ::jxx::Ptr<::jxx::lang::String>& uri) override;

    ::jxx::Ptr<::jxx::lang::String> writeToString(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg) override;

private:
    ::jxx::Ptr<::jxx::lang::String> newLine_;
    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializerFilter> filter_;
};

} // namespace jxx::org::w3c::dom::ls::internal
