#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::w3c::dom {

class Node;

class DOMLocator
    : public ::jxx::lang::InterfaceBase<DOMLocator> {
public:
    ~DOMLocator() override = default;

    virtual ::jxx::lang::jint getLineNumber() const = 0;
    virtual ::jxx::lang::jint getColumnNumber() const = 0;
    virtual ::jxx::lang::jint getByteOffset() const = 0;
    virtual ::jxx::lang::jint getUtf16Offset() const = 0;
    virtual ::jxx::Ptr<Node> getRelatedNode() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getUri() const = 0;
};

} // namespace jxx::org::w3c::dom
