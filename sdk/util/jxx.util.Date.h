#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util {

class Date : public jxx::lang::Object {
public:
    Date();
    explicit Date(jxx::lang::jlong epochMillis);

    jxx::lang::jlong getTime() const;
    void setTime(jxx::lang::jlong epochMillis);

private:
    jxx::lang::jlong time_ = 0;
};

} // namespace jxx::util
