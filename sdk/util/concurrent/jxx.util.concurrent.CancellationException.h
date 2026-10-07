#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::util::concurrent {
class CancellationException : public jxx::lang::IllegalStateException {
public:
    using JxxSuper = jxx::lang::IllegalStateException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<CancellationException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    CancellationException() = default;

    CancellationException(const CancellationException&) = default;
    CancellationException(CancellationException&&) noexcept = default;
    CancellationException& operator=(const CancellationException&) = default;
    CancellationException& operator=(CancellationException&&) noexcept = default;
    ~CancellationException() override = default;
 public: using jxx::lang::IllegalStateException::IllegalStateException; protected: JXX_OBJECT_CLONE(CancellationException) };
}
