#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.Deque.h"
#include "util/jxx.util.concurrent.BlockingQueue.h"

namespace jxx::util::concurrent {

template<typename E>
class BlockingDeque
    : public ::jxx::lang::InterfaceBase<
          BlockingDeque<E>,
          BlockingQueue<E>,
          ::jxx::util::Deque<E>> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        BlockingDeque<E>, BlockingQueue<E>, ::jxx::util::Deque<E>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~BlockingDeque() override = default;

    virtual void putFirst(const ::jxx::Ptr<E>& element) = 0;
    virtual void putLast(const ::jxx::Ptr<E>& element) = 0;
    virtual ::jxx::lang::jbool offerFirst(const ::jxx::Ptr<E>& element,
        ::jxx::lang::jlong timeout, const ::jxx::Ptr<TimeUnit>& unit) = 0;
    virtual ::jxx::lang::jbool offerLast(const ::jxx::Ptr<E>& element,
        ::jxx::lang::jlong timeout, const ::jxx::Ptr<TimeUnit>& unit) = 0;
    virtual ::jxx::Ptr<E> takeFirst() = 0;
    virtual ::jxx::Ptr<E> takeLast() = 0;
    virtual ::jxx::Ptr<E> pollFirst(::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
    virtual ::jxx::Ptr<E> pollLast(::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
};

} // namespace jxx::util::concurrent
