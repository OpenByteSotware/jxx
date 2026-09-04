#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"
namespace jxx::util::concurrent {
class ExecutionException : public jxx::lang::Exception {
public:
    using JxxSuper = jxx::lang::Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ExecutionException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ExecutionException() = default;

    ExecutionException(const ExecutionException&) = default;
    ExecutionException(ExecutionException&&) noexcept = default;
    ExecutionException& operator=(const ExecutionException&) = default;
    ExecutionException& operator=(ExecutionException&&) noexcept = default;
    ~ExecutionException() override = default;
 public: using jxx::lang::Exception::Exception; protected: JXX_OBJECT_CLONE(ExecutionException) };
}
