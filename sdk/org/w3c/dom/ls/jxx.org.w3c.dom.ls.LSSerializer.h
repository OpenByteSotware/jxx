#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::w3c::dom {
class DOMConfiguration;
class Node;
}

namespace jxx::org::w3c::dom::ls {

class LSOutput;
class LSSerializerFilter;

class LSSerializer {
public:
    virtual ~LSSerializer() = default;

    virtual ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration> getDomConfig() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getNewLine() const = 0;
    virtual void setNewLine(const ::jxx::Ptr<::jxx::lang::String>& newLine) = 0;
    virtual ::jxx::Ptr<LSSerializerFilter> getFilter() const = 0;
    virtual void setFilter(const ::jxx::Ptr<LSSerializerFilter>& filter) = 0;

    virtual ::jxx::lang::jbool write(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg,
        const ::jxx::Ptr<LSOutput>& destination) = 0;

    virtual ::jxx::lang::jbool writeToURI(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg,
        const ::jxx::Ptr<::jxx::lang::String>& uri) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> writeToString(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg) = 0;
};

} // namespace jxx::org::w3c::dom::ls
