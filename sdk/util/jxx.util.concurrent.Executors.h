#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.concurrent.ExecutorService.h"
#include "util/jxx.util.concurrent.ThreadFactory.h"
namespace jxx::util::concurrent {
class Executors final : public jxx::lang::Object {
private: Executors() = delete;
public:
    static jxx::Ptr<ExecutorService> newFixedThreadPool(jxx::lang::jint threadCount);
    static jxx::Ptr<ExecutorService> newSingleThreadExecutor();
    static jxx::Ptr<ExecutorService> newCachedThreadPool();
protected: jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
};
} // namespace
