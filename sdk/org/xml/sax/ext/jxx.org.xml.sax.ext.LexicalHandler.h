#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax::ext {

class LexicalHandler
    : public ::jxx::lang::InterfaceBase<LexicalHandler> {
public:
    ~LexicalHandler() override = default;

    virtual void startDTD(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;

    virtual void endDTD() = 0;

    virtual void startEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name) = 0;

    virtual void endEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name) = 0;

    virtual void startCDATA() = 0;
    virtual void endCDATA() = 0;

    virtual void comment(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) = 0;
};

} // namespace jxx::org::xml::sax::ext
