#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::util::concurrent {
template<typename V>
class Callable : public ::jxx::lang::InterfaceBase<Callable<V>> {
public:
    using Super = ::jxx::lang::InterfaceBase<Callable<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~Callable() override = default;
    virtual ::jxx::Ptr<V> call() = 0;
};
} // namespace jxx::util::concurrent
