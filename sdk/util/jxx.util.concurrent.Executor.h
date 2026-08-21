#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Runnable.h"
namespace jxx::util::concurrent {
class Executor {
public:
    virtual ~Executor() = default;
    virtual void execute(const jxx::Ptr<jxx::lang::Runnable>& command) = 0;
};
} // namespace jxx::util::concurrent
