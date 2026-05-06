#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util {

class Date : public jxx::lang::Object {
public:
    Date();
    explicit Date(jlong epochMillis);

    jlong getTime() const;
    void setTime(jlong epochMillis);

private:
    jlong time_ = 0;
};

} // namespace jxx::util
