// integer.hpp
#pragma once
#include "jxx.lang.Object.h"

namespace jxx {
    namespace lang {

class Integer : public Object, Cloneable {
    int v_{ 0 };
public:
    explicit Integer(int v) : v_(v) {}
    int intValue() const noexcept { return v_; }

    virtual jbool equals(const JXX_PTR(Object) other) const noexcept override {
        auto o = JXX_CAST(Integer, other);
        return o && v_ == o->v_;
    }
    virtual jint hashCode() const noexcept override {
        return std::hash<int>{}(v_);
    }
    virtual std::string toString() const override {
        return std::to_string(v_);
    }

protected:
    // Implement cloneImpl for deep copy, Ojbect uses this for C++ to mimic java like clone
    virtual std::shared_ptr<Object> cloneImpl() const override {
        return JXX_NEW<Integer>(this->v_);
    };
};

    } // namespace lang
} // namespace jxx
