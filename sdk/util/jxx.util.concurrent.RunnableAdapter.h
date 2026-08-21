#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Callable.h"
namespace jxx::util::concurrent {
template <typename V>
class RunnableAdapter final
    : public jxx::lang::Object
    , public virtual Callable<V> {
private:
    jxx::Ptr<jxx::lang::Runnable> task_;
    jxx::Ptr<V> result_;
public:
    RunnableAdapter(
        const jxx::Ptr<jxx::lang::Runnable>& task,
        const jxx::Ptr<V>& result)
        : task_(task), result_(result) {
        if (task_ == nullptr) throw jxx::lang::NullPointerException();
    }
    jxx::Ptr<V> call() override { task_->run(); return result_; }
protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        throw jxx::lang::CloneNotSupportedException();
    }
};
} // namespace jxx::util::concurrent
