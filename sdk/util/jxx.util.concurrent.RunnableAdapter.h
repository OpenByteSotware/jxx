#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Callable.h"
namespace jxx::util::concurrent {
template<typename V>
class RunnableAdapter final
    : public ::jxx::lang::ClassBase<RunnableAdapter<V>, ::jxx::lang::Object, Callable<V>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<RunnableAdapter<V>, JxxSuper, Callable<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() { return JxxClassInfoMarker::Class(); }

    RunnableAdapter(const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<V>& result)
        : Super(), task_(task), result_(result) {
        if (task_ == nullptr) throw ::jxx::lang::NullPointerException();
    }
    ::jxx::Ptr<V> call() override { task_->run(); return result_; }
private:
    ::jxx::Ptr<::jxx::lang::Runnable> task_;
    ::jxx::Ptr<V> result_;
};
} // namespace jxx::util::concurrent
