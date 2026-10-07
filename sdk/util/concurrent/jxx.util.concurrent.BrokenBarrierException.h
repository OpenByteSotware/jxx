#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::util::concurrent {
class BrokenBarrierException : public jxx::lang::Exception {
public:
    using JxxSuper = jxx::lang::Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<BrokenBarrierException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    BrokenBarrierException() = default;

    BrokenBarrierException(const BrokenBarrierException&) = default;
    BrokenBarrierException(BrokenBarrierException&&) noexcept = default;
    BrokenBarrierException& operator=(const BrokenBarrierException&) = default;
    BrokenBarrierException& operator=(BrokenBarrierException&&) noexcept = default;
    ~BrokenBarrierException() override = default;
 public: using jxx::lang::Exception::Exception; protected: JXX_OBJECT_CLONE(BrokenBarrierException) };
}
