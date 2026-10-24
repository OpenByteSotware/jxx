#pragma once

#include "lang/jxx.lang.AutoCloseable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx_types.h"

namespace jxx::util { template<typename T> class Iterator; template<typename T> class Spliterator; }

namespace jxx::util::stream {

template<typename T, typename S>
class BaseStream
    : public ::jxx::lang::InterfaceBase<
          BaseStream<T, S>,
          ::jxx::lang::AutoCloseable> {
public:
    ~BaseStream() override = default;
    virtual ::jxx::Ptr<::jxx::util::Iterator<T>> iterator() = 0;
    virtual ::jxx::Ptr<::jxx::util::Spliterator<T>> spliterator() = 0;
    virtual ::jxx::lang::jbool isParallel() = 0;
    virtual ::jxx::Ptr<S> sequential() = 0;
    virtual ::jxx::Ptr<S> parallel() = 0;
    virtual ::jxx::Ptr<S> unordered() = 0;
    virtual ::jxx::Ptr<S> onClose(
        const ::jxx::Ptr<::jxx::lang::Runnable>& closeHandler) = 0;
    void close() override = 0;
};

} // namespace jxx::util::stream
