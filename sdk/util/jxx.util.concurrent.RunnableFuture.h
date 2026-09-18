#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Future.h"

namespace jxx::util::concurrent {

template<typename V>
class RunnableFuture
    : public ::jxx::lang::InterfaceBase<
          RunnableFuture<V>,
          ::jxx::lang::Runnable,
          Future<V>> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        RunnableFuture<V>, ::jxx::lang::Runnable, Future<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~RunnableFuture() override = default;
};

} // namespace jxx::util::concurrent
