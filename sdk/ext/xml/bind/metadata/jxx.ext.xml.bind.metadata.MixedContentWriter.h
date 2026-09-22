#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::metadata {
class MixedContentWriter : public ::jxx::lang::InterfaceBase<MixedContentWriter> {
public:
    ~MixedContentWriter() override = default;
    virtual void addText(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::String>& text) = 0;
    virtual void addElement(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
};
} // namespace jxx::ext::xml::bind::metadata
