#pragma once

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net {

template<typename T>
class SocketOption
    : public ::jxx::lang::InterfaceBase<SocketOption<T>> {
public:
    ~SocketOption() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String> name() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::ClassAny> type() const = 0;
};

class BasicSocketOption final
    : public ::jxx::lang::ClassBase<
          BasicSocketOption,
          ::jxx::lang::Object,
          SocketOption<::jxx::lang::Object>> {
public:
    BasicSocketOption(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type);

    ::jxx::Ptr<::jxx::lang::String> name() const override;
    ::jxx::Ptr<::jxx::lang::ClassAny> type() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::ClassAny> type_;
};

} // namespace jxx::net
