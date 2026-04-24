// integer.hpp
#pragma once
#include "jxx_types.h"
#include "jxx.lang.Object.h"

namespace jxx {
    namespace lang {

class Integer : public Object, Cloneable {
    int v_{ 0 };
public:
    explicit Integer(int v) : v_(v) {}
    jint intValue() const noexcept { return v_; }

    virtual jbool equals(const JXX_PTR(Object) other) const noexcept override {
        auto o = JXX_CAST_PTR(Integer, other);
        return o && v_ == o->v_;
    }
    virtual jint hashCode() const noexcept override {
        return std::hash<int>{}(v_);
    }
    virtual JXX_PTR(String) toString() const override {
        return JXX_NEW<String>(std::to_string(v_));
    }

protected:
    // Implement cloneImpl for deep copy, Ojbect uses this for C++ to mimic java like clone
    virtual JXX_PTR(Object ) cloneImpl() const override {
        return JXX_NEW<Integer>(this->v_);
    };
};

    } // namespace lang
} // namespace jxx
