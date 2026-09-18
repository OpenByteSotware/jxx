#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.Future.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

template<typename V>
class CompletionService
    : public ::jxx::lang::InterfaceBase<CompletionService<V>> {
public:
    using Super = ::jxx::lang::InterfaceBase<CompletionService<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~CompletionService() override = default;

    virtual ::jxx::Ptr<Future<V>> submit(
        const ::jxx::Ptr<Callable<V>>& task) = 0;
    virtual ::jxx::Ptr<Future<V>> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<V>& result) = 0;
    virtual ::jxx::Ptr<Future<V>> take() = 0;
    virtual ::jxx::Ptr<Future<V>> poll() = 0;
    virtual ::jxx::Ptr<Future<V>> poll(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
};

} // namespace jxx::util::concurrent
