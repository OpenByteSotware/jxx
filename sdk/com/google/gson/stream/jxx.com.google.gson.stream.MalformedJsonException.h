#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.IOException.h"
namespace com::google::gson::stream {
class MalformedJsonException : public ::jxx::lang::ClassBase<MalformedJsonException, ::jxx::io::IOException> {
public:
    using JxxSuper = ::jxx::io::IOException;
    using Super = ::jxx::lang::ClassBase<MalformedJsonException, JxxSuper>;

    MalformedJsonException();
    explicit MalformedJsonException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
};
} // namespace com::google::gson::stream
