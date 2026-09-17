#pragma once

#include <functional>

#include "lang/jxx.lang.ThreadLocal.h"
#include "lang/jxx.lang.ThreadLocalSupport.h"

namespace jxx::lang {

template <typename T>
class InheritableThreadLocal
    : public ThreadLocal<T>
    , private thread_local_detail::InheritableEntry {
public:
    using JxxSuper = ThreadLocal<T>;
    using Super = JxxSuper;

    InheritableThreadLocal()
        : JxxSuper() {
        thread_local_detail::addInheritableEntry(this);
    }

    ~InheritableThreadLocal() override {
        thread_local_detail::removeInheritableEntry(this);
    }

protected:
    virtual jxx::Ptr<T> childValue(const jxx::Ptr<T>& parentValue) {
        return parentValue;
    }

private:
    std::function<void()> captureForChild() override {
        jbool present = false;
        const auto parentValue = this->currentValueIfPresent(present);
        if (!present) return {};
        const auto value = childValue(parentValue);
        return [this, value] { this->setForCurrentThread(value); };
    }
};

} // namespace jxx::lang
