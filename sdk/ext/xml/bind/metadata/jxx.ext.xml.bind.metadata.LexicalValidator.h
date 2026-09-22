#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::metadata {
class LexicalValidator
    : public ::jxx::lang::InterfaceBase<LexicalValidator> {
public:
    ~LexicalValidator() override = default;
    virtual void validate(
        const ::jxx::Ptr<::jxx::lang::String>& lexicalValue) = 0;
};
} // namespace jxx::ext::xml::bind::metadata
