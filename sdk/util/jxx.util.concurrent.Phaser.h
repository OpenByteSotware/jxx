#pragma once

#include <condition_variable>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

class Phaser
    : public ::jxx::lang::ClassBase<Phaser, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Phaser, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<Phaser, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
    Phaser();
    explicit Phaser(::jxx::lang::jint parties);
    explicit Phaser(const ::jxx::Ptr<Phaser>& parent);
    Phaser(const ::jxx::Ptr<Phaser>& parent, ::jxx::lang::jint parties);
    ~Phaser() override = default;

    ::jxx::lang::jint registerParty();
    ::jxx::lang::jint bulkRegister(::jxx::lang::jint parties);
    ::jxx::lang::jint arrive();
    ::jxx::lang::jint arriveAndDeregister();
    ::jxx::lang::jint arriveAndAwaitAdvance();
    ::jxx::lang::jint awaitAdvance(::jxx::lang::jint phase);
    ::jxx::lang::jint awaitAdvanceInterruptibly(::jxx::lang::jint phase);
    ::jxx::lang::jint awaitAdvanceInterruptibly(::jxx::lang::jint phase,
        ::jxx::lang::jlong timeout, const ::jxx::Ptr<TimeUnit>& unit);
    void forceTermination();
    ::jxx::lang::jbool isTerminated() const;
    ::jxx::lang::jint getPhase() const;
    ::jxx::lang::jint getRegisteredParties() const;
    ::jxx::lang::jint getArrivedParties() const;
    ::jxx::lang::jint getUnarrivedParties() const;
    ::jxx::Ptr<Phaser> getParent() const;
    ::jxx::Ptr<Phaser> getRoot() const;

protected:
    virtual ::jxx::lang::jbool onAdvance(::jxx::lang::jint phase,
        ::jxx::lang::jint registeredParties);

private:
    ::jxx::lang::jint doArrive_(::jxx::lang::jbool deregister);
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    ::jxx::Ptr<Phaser> parent_;
    ::jxx::lang::jint phase_ = 0;
    ::jxx::lang::jint registered_ = 0;
    ::jxx::lang::jint unarrived_ = 0;
    ::jxx::lang::jbool terminated_ = false;
};

} // namespace jxx::util::concurrent
