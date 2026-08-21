#pragma once
#include "lang/jxx_types.h"
namespace jxx::util::concurrent {
template <typename V> class Callable {
public:
    virtual ~Callable() = default;
    virtual jxx::Ptr<V> call() = 0;
};
} // namespace jxx::util::concurrent
