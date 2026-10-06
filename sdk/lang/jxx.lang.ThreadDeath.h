#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Error.h"

namespace jxx::lang {

class ThreadDeath : public ::jxx::lang::ClassBase<ThreadDeath, Error> {
public:
    using JxxSuper = Error;
    using Super = ::jxx::lang::ClassBase<ThreadDeath, JxxSuper>;

    ThreadDeath() = default;
    ThreadDeath(const ThreadDeath&) = default;
    ThreadDeath(ThreadDeath&&) noexcept = default;
    ThreadDeath& operator=(const ThreadDeath&) = default;
    ThreadDeath& operator=(ThreadDeath&&) noexcept = default;
    ~ThreadDeath() override = default;

protected:
    JXX_OBJECT_CLONE(ThreadDeath)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
