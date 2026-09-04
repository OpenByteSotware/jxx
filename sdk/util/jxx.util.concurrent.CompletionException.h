#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"
namespace jxx::util::concurrent {

class CompletionException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<CompletionException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    CompletionException() = default;

    CompletionException(const CompletionException&) = default;
    CompletionException(CompletionException&&) noexcept = default;
    CompletionException& operator=(const CompletionException&) = default;
    CompletionException& operator=(CompletionException&&) noexcept = default;
    ~CompletionException() override = default;
 public: using jxx::lang::RuntimeException::RuntimeException; protected: JXX_OBJECT_CLONE(CompletionException) };
}
