#pragma once

#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::io {

class Reader
    : public ::jxx::lang::ClassBase<
          Reader,
          ::jxx::lang::Object,
          Closeable> {
public:
    using JxxSuper = ::jxx::lang::Object;

    using Super =
        ::jxx::lang::ClassBase<
            Reader,
            JxxSuper,
            Closeable>;

    ~Reader() override = default;

    virtual ::jxx::lang::jint read();

    virtual ::jxx::lang::jint read(
        const ::jxx::lang::CharArray& buffer);

    virtual ::jxx::lang::jint read(
        const ::jxx::lang::CharArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) = 0;

    virtual ::jxx::lang::jlong skip(
        ::jxx::lang::jlong count);

    virtual ::jxx::lang::jbool ready();

    virtual ::jxx::lang::jbool
    markSupported() const;

    virtual void mark(
        ::jxx::lang::jint readAheadLimit);

    virtual void reset();

    virtual void close() override = 0;

protected:
    /**
     * Creates a reader with a dedicated synchronization monitor.
     */
    Reader();

    /**
     * Creates a reader that synchronizes on the supplied monitor.
     */
    explicit Reader(
        const ::jxx::Ptr<::jxx::lang::Object>& lockObject);

    /**
2
* Synchronization object used by reader operations.
3
*
4
* The no-argument constructor creates a dedicated monitor object.
5
* The explicit constructor retains the supplied monitor object.
6
*
7
* A dedicated monitor is used instead of a counted self-reference
8
* because the owning jxx::Ptr is established only after construction,
9
* and storing that counted self-reference would create an ownership
10
* cycle.
11
*/
    ::jxx::Ptr<::jxx::lang::Object> lock;
};

} // namespace jxx::io
