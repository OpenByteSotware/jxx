#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax::ext {

class DeclHandler
    : public ::jxx::lang::InterfaceBase<DeclHandler> {
public:
    ~DeclHandler() override = default;

    virtual void elementDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& model) = 0;

    virtual void attributeDecl(
        const ::jxx::Ptr<::jxx::lang::String>& elementName,
        const ::jxx::Ptr<::jxx::lang::String>& attributeName,
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& mode,
        const ::jxx::Ptr<::jxx::lang::String>& value) = 0;

    virtual void internalEntityDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& value) = 0;

    virtual void externalEntityDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;
};

} // namespace jxx::org::xml::sax::ext
