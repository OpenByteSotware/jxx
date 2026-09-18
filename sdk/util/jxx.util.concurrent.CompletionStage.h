#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.Function.h"

namespace jxx::lang { class Throwable; class Void; }
namespace jxx::util::concurrent {
template<typename T> class CompletableFuture;

template<typename T>
class CompletionStage
    : public ::jxx::lang::InterfaceBase<CompletionStage<T>> {
public:
    using Super = ::jxx::lang::InterfaceBase<CompletionStage<T>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~CompletionStage() override = default;

    template<typename U>
    ::jxx::Ptr<CompletionStage<U>> thenApply(
        const ::jxx::Ptr<::jxx::util::function::Function<T,U>>& function);
    ::jxx::Ptr<CompletionStage<::jxx::lang::Void>> thenAccept(
        const ::jxx::Ptr<::jxx::util::function::Consumer<T>>& action);
    ::jxx::Ptr<CompletionStage<::jxx::lang::Void>> thenRun(
        const ::jxx::Ptr<::jxx::lang::Runnable>& action);
    ::jxx::Ptr<CompletionStage<T>> exceptionally(
        const ::jxx::Ptr<::jxx::util::function::Function<::jxx::lang::Throwable,T>>& function);
    template<typename U>
    ::jxx::Ptr<CompletionStage<U>> handle(
        const ::jxx::Ptr<::jxx::util::function::BiFunction<T,::jxx::lang::Throwable,U>>& function);
    ::jxx::Ptr<CompletionStage<T>> whenComplete(
        const ::jxx::Ptr<::jxx::util::function::BiConsumer<T,::jxx::lang::Throwable>>& action);
    virtual ::jxx::Ptr<CompletableFuture<T>> toCompletableFuture() = 0;
};

} // namespace jxx::util::concurrent
