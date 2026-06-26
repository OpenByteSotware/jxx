#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {
namespace concurrent {

template <typename V>
class Callable : virtual public jxx::lang::Object {
public:
    virtual ~Callable() = default;

    // Java 8: V call() throws Exception;
    virtual jxx::Ptr<V> call() = 0;
};

} // namespace concurrent
} // namespace util
} // namespace jxx
