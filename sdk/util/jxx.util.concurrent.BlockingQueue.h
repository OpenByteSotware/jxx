#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.Queue.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

template<typename E>
class BlockingQueue
    : public ::jxx::lang::InterfaceBase<
          BlockingQueue<E>,
          ::jxx::util::Queue<E>> {
public:
    using JxxSuper = ::jxx::util::Queue<E>;
    using Super = ::jxx::lang::InterfaceBase<
        BlockingQueue<E>,
        JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    ~BlockingQueue() override = default;

    virtual void put(const ::jxx::Ptr<E>& element) = 0;
    virtual ::jxx::lang::jbool offer(
        const ::jxx::Ptr<E>& element,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
    virtual ::jxx::Ptr<E> take() = 0;
    virtual ::jxx::Ptr<E> poll(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
    virtual ::jxx::lang::jint remainingCapacity() = 0;
    virtual ::jxx::lang::jint drainTo(
        const ::jxx::Ptr<::jxx::util::Collection<E>>& target) = 0;
    virtual ::jxx::lang::jint drainTo(
        const ::jxx::Ptr<::jxx::util::Collection<E>>& target,
        ::jxx::lang::jint maximumElements) = 0;
};

} // namespace jxx::util::concurrent
