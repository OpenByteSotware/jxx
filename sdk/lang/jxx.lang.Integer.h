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

    virtual jbool equals(const jxx::Ptr<Object> other) const override {
        auto o = JXX_CAST_PTR(Integer, other);
        return o && v_ == o->v_;
    }
    virtual jint hashCode() const override {
        return std::hash<int>{}(v_);
    }
    virtual jxx::Ptr<String> toString() const override {
        return JXX_NEW<String>(std::to_string(v_));
    }

protected:
    // Implement cloneImpl for deep copy, Object uses this for C++ to mimic java like clone
    virtual jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        return JXX_NEW<Integer>(this->v_);
    }
};
    
    } // namespace lang
} // namespace jxx
