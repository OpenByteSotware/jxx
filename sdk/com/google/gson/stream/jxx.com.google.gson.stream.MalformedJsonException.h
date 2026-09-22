#pragma once
#include "io/jxx.io.IOException.h"
namespace com::google::gson::stream {
class MalformedJsonException : public ::jxx::io::IOException {
public:
    MalformedJsonException();
    explicit MalformedJsonException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
};
} // namespace com::google::gson::stream
