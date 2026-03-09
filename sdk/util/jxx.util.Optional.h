
#pragma once
#include <optional>
#include <functional>
#include "jxx.lang.NullPointerException.h"

namespace jxx { namespace util {

template <typename T>
class Optional {
public:
    static Optional empty() { return Optional(); }
    static Optional of(const T& value) { return Optional(value); }
    static Optional ofNullable(const T* value) { return value? Optional(*value) : Optional(); }

    bool isPresent() const noexcept { return val_.has_value(); }
    T get() const { if (!val_) throw jxx::lang::NullPointerException(); return *val_; }

    template <typename F> Optional map(F f) const { return val_? Optional(f(*val_)) : Optional(); }
    T orElse(const T& other) const { return val_.value_or(other); }

private:
    Optional() = default; explicit Optional(const T& v) : val_(v) {}
    std::optional<T> val_;
};

}} // namespace jxx::util
