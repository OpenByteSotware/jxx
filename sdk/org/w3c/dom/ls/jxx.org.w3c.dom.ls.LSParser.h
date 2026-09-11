#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::w3c::dom {
class DOMConfiguration;
class Document;
class Node;
}

namespace jxx::org::w3c::dom::ls {

class LSInput;
class LSParserFilter;

class LSParser {
public:
    virtual ~LSParser() = default;

    enum : ::jxx::lang::jshort {
        ACTION_APPEND_AS_CHILDREN = 1,
        ACTION_REPLACE_CHILDREN = 2,
        ACTION_INSERT_BEFORE = 3,
        ACTION_INSERT_AFTER = 4,
        ACTION_REPLACE = 5
    };

    virtual ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration> getDomConfig() const = 0;
    virtual ::jxx::Ptr<LSParserFilter> getFilter() const = 0;
    virtual void setFilter(const ::jxx::Ptr<LSParserFilter>& filter) = 0;
    virtual ::jxx::lang::jbool getAsync() const = 0;
    virtual ::jxx::lang::jbool getBusy() const = 0;

    virtual ::jxx::Ptr<::jxx::org::w3c::dom::Document> parse(
        const ::jxx::Ptr<LSInput>& input) = 0;

    virtual ::jxx::Ptr<::jxx::org::w3c::dom::Document> parseURI(
        const ::jxx::Ptr<::jxx::lang::String>& uri) = 0;

    virtual ::jxx::Ptr<::jxx::org::w3c::dom::Node> parseWithContext(
        const ::jxx::Ptr<LSInput>& input,
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& contextArg,
        ::jxx::lang::jshort action) = 0;

    virtual void abort() = 0;
};

} // namespace jxx::org::w3c::dom::ls
