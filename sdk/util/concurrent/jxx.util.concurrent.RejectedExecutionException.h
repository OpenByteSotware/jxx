#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"
namespace jxx::util::concurrent {
class RejectedExecutionException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<RejectedExecutionException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    RejectedExecutionException() = default;

    RejectedExecutionException(const RejectedExecutionException&) = default;
    RejectedExecutionException(RejectedExecutionException&&) noexcept = default;
    RejectedExecutionException& operator=(const RejectedExecutionException&) = default;
    RejectedExecutionException& operator=(RejectedExecutionException&&) noexcept = default;
    ~RejectedExecutionException() override = default;
 public: using jxx::lang::RuntimeException::RuntimeException; protected: JXX_OBJECT_CLONE(RejectedExecutionException) };
}
