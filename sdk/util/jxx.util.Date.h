#pragma once
#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
namespace jxx::util {
class Date : public jxx::lang::Object {
public:
    Date();
    explicit Date(jlong epochMillis);
    jlong getTime() const;
    void setTime(jlong epochMillis);
    jxx::Ptr<jxx::lang::String> toString() const override;
private:
    jlong time_ = 0;
};
}