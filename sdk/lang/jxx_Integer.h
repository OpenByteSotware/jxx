// integer.hpp
#pragma once
#include "jxx_object.h"

namespace jxx {
    namespace lang {

class Integer : public Cloneable<Integer> {
    int v_{ 0 };
public:
    explicit Integer(int v) : v_(v) {}
    int intValue() const noexcept { return v_; }

    bool equals(const Object& other) const noexcept override {
        auto* o = dynamic_cast<const Integer*>(&other);
        return o && v_ == o->v_;
    }
    std::size_t hashCode() const noexcept override {
        return std::hash<int>{}(v_);
    }
    std::string toString() const override {
        return std::to_string(v_);
    }
};

    } // namespace lang
} // namespace jxx
