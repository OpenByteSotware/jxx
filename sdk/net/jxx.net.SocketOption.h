#pragma once

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include <functional>

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
    using TypeResolver =
        std::function<::jxx::Ptr<::jxx::lang::ClassAny>()>;

    BasicSocketOption(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        TypeResolver typeResolver);

    ::jxx::Ptr<::jxx::lang::String> name() const override;
    ::jxx::Ptr<::jxx::lang::ClassAny> type() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    TypeResolver typeResolver_;
};

} // namespace jxx::net
