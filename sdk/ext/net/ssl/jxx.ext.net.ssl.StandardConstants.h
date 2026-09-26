#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::net::ssl {
class StandardConstants final
    : public ::jxx::lang::ClassBase<
          StandardConstants,
          ::jxx::lang::Object> {
public:
    static constexpr ::jxx::lang::jint SNI_HOST_NAME = 0;
private:
    StandardConstants() = delete;
};
} // namespace jxx::ext::net::ssl
