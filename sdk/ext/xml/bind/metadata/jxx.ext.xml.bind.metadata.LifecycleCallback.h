#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::metadata {

class LifecycleCallback
    : public ::jxx::lang::InterfaceBase<LifecycleCallback> {
public:
    ~LifecycleCallback() override = default;

    virtual void beforeUnmarshal(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::Object>& parent) = 0;

    virtual void afterUnmarshal(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::Object>& parent) = 0;

    virtual void beforeMarshal(
        const ::jxx::Ptr<::jxx::lang::Object>& source) = 0;

    virtual void afterMarshal(
        const ::jxx::Ptr<::jxx::lang::Object>& source) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
