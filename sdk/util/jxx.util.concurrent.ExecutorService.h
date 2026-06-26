#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.concurrent.Executor.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.Future.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "util/jxx.util.concurrent.wildcard.FutureAny.h"

namespace jxx {
namespace util {

template <typename E> class List;
namespace wildcard { template <typename E> class CollectionExtends; }

namespace concurrent {

class ExecutorService : public virtual Executor {
public:
    virtual ~ExecutorService() = default;

    virtual void shutdown() = 0;
    virtual jxx::Ptr<jxx::util::List<jxx::lang::Runnable>> shutdownNow() = 0;
    virtual jxx::lang::jbool isShutdown() = 0;
    virtual jxx::lang::jbool isTerminated() = 0;
    virtual jxx::lang::jbool awaitTermination(
        jxx::lang::jlong timeout,
        jxx::Ptr<TimeUnit> unit) = 0;

    template <typename T>
    jxx::Ptr<Future<T>> submit(jxx::Ptr<Callable<T>> /*task*/) {
        throw UnsupportedOperationException();
    }

    template <typename T>
    jxx::Ptr<Future<T>> submit(
        jxx::Ptr<jxx::lang::Runnable> /*task*/,
        jxx::Ptr<T> /*result*/) {
        throw UnsupportedOperationException();
    }

    virtual jxx::Ptr<wildcard::FutureAny> submit(
        jxx::Ptr<jxx::lang::Runnable> task) = 0;

    template <typename T>
    jxx::Ptr<jxx::util::List<Future<T>>> invokeAll(
        jxx::Ptr<jxx::util::wildcard::CollectionExtends<Callable<T>>> /*tasks*/) {
        throw UnsupportedOperationException();
    }

    template <typename T>
    jxx::Ptr<jxx::util::List<Future<T>>> invokeAll(
        jxx::Ptr<jxx::util::wildcard::CollectionExtends<Callable<T>>> /*tasks*/,
        jxx::lang::jlong /*timeout*/,
        jxx::Ptr<TimeUnit> /*unit*/) {
        throw UnsupportedOperationException();
    }

    template <typename T>
    jxx::Ptr<T> invokeAny(
        jxx::Ptr<jxx::util::wildcard::CollectionExtends<Callable<T>>> /*tasks*/) {
        throw UnsupportedOperationException();
    }

    template <typename T>
    jxx::Ptr<T> invokeAny(
        jxx::Ptr<jxx::util::wildcard::CollectionExtends<Callable<T>>> /*tasks*/,
        jxx::lang::jlong /*timeout*/,
        jxx::Ptr<TimeUnit> /*unit*/) {
        throw UnsupportedOperationException();
    }
};

} // namespace concurrent
} // namespace util
} // namespace jxx
