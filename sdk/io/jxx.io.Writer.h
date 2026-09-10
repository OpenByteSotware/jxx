#pragma once

#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Flushable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class CharSequence;
class String;

} // namespace jxx::lang

namespace jxx::io {

class Writer
    : public ::jxx::lang::ClassBase<
          Writer,
          ::jxx::lang::Object,
          Closeable,
          Flushable> {
public:
    using JxxSuper = ::jxx::lang::Object;

    using Super =
        ::jxx::lang::ClassBase<
            Writer,
            JxxSuper,
            Closeable,
            Flushable>;

    ~Writer() override = default;

    virtual void write(
        ::jxx::lang::jint value);

    virtual void write(
        const ::jxx::lang::CharArray& buffer);

    virtual void write(
        const ::jxx::lang::CharArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) = 0;

    virtual void write(
        const ::jxx::Ptr<::jxx::lang::String>& value);

    virtual void write(
        const ::jxx::Ptr<::jxx::lang::String>& value,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length);

    virtual ::jxx::Ptr<Writer> append(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& sequence);

    virtual ::jxx::Ptr<Writer> append(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& sequence,
        ::jxx::lang::jint start,
        ::jxx::lang::jint end);

    virtual ::jxx::Ptr<Writer> append(
        ::jxx::lang::jchar value);

    virtual void flush() override = 0;
    virtual void close() override = 0;

protected:
    /**
     * Creates a writer with a dedicated synchronization monitor.
     */
    Writer();

    /**
     * Creates a writer that synchronizes on the supplied monitor.
     */
    explicit Writer(
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
