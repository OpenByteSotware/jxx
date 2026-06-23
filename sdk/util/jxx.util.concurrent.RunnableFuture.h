#pragma once

#include "io/jxx.lang.Runnable.h"
#include "io/jxx.util.concurrent.Future.h"

namespace jxx {
namespace util {
namespace concurrent {

template <typename V>
class RunnableFuture
    : public virtual jxx::lang::Runnable
    , public virtual Future<V> {
public:
    virtual ~RunnableFuture() = default;
};

} // namespace concurrent
} // namespace util
} // namespace jxx
