#pragma once

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.EventListener.h"

namespace jxx::util {

template <typename T>
class EventListenerProxy
    : public ::jxx::lang::ClassBase<
          EventListenerProxy<T>,
          ::jxx::lang::Object,
          EventListener> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        EventListenerProxy<T>, JxxSuper, EventListener>;

    explicit EventListenerProxy(const ::jxx::Ptr<T>& listener)
        : Super(), listener_(listener) {
        if (listener_ == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
    }

    ~EventListenerProxy() override = default;

    ::jxx::Ptr<T> getListener() const {
        return listener_;
    }

private:
    ::jxx::Ptr<T> listener_;
};

} // namespace jxx::util
