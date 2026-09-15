#pragma once

#include "lang/jxx.lang.Error.h"

namespace jxx::lang {

class ThreadDeath : public Error {
public:
    using JxxSuper = Error;

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
