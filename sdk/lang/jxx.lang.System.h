
#pragma once
#include <memory>
#include <vector>
#include <cstring>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include <chrono>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"

namespace jxx { namespace lang {
class Object;
class String;
struct System {
    static std::shared_ptr<jxx::io::InputStream> in;
    static std::shared_ptr<jxx::io::PrintStream> out;
    static std::shared_ptr<jxx::io::PrintStream> err;

    static void init();
    static void setIn(std::shared_ptr<jxx::io::InputStream> s) { in = std::move(s); }
    static void setOut(std::shared_ptr<jxx::io::PrintStream> s) { out = std::move(s); }
    static void setErr(std::shared_ptr<jxx::io::PrintStream> s) { err = std::move(s); }

    // Java-like utilities
    static jxx::lang::jlong currentTimeMillis();
    static jxx::lang::jlong nanoTime();
    static jxx::lang::jint identityHashCode(
        const jxx::Ptr<jxx::lang::Object>& object) noexcept;
    static jxx::Ptr<jxx::lang::String> lineSeparator();
    static jxx::Ptr<jxx::lang::String> getenv(
        const jxx::Ptr<jxx::lang::String>& name);
    static jxx::Ptr<jxx::lang::String> mapLibraryName(
        const jxx::Ptr<jxx::lang::String>& libraryName);
    static void gc();
    static void runFinalization();
    static void exit(jxx::lang::jint status);

    static jxx::Ptr<jxx::lang::String> getProperty(
        const jxx::Ptr<jxx::lang::String>& key);
    static jxx::Ptr<jxx::lang::String> getProperty(
        const jxx::Ptr<jxx::lang::String>& key,
        const jxx::Ptr<jxx::lang::String>& defaultValue);
    static jxx::Ptr<jxx::lang::String> setProperty(
        const jxx::Ptr<jxx::lang::String>& key,
        const jxx::Ptr<jxx::lang::String>& value);
    static jxx::Ptr<jxx::lang::String> clearProperty(
        const jxx::Ptr<jxx::lang::String>& key);

    template<typename T>
    static void arraycopy(const std::vector<T>& src, jxx::lang::jint srcPos,
                          std::vector<T>& dest, jxx::lang::jint destPos,
                          jxx::lang::jint length) {
        if (srcPos < 0 || destPos < 0 || length < 0 ||
            (size_t)srcPos + (size_t)length > src.size() ||
            (size_t)destPos + (size_t)length > dest.size()) {
            throw jxx::lang::IndexOutOfBoundsException("System.arraycopy: index out of bounds");
        }
        if (length == 0) return;
        std::memmove(&dest[(size_t)destPos],
                     &src[(size_t)srcPos],
                     (size_t)length * sizeof(T));
    }

    static void arraycopy(const jxx::lang::jchar* src, jxx::lang::jint srcPos,
                          jxx::lang::jchar* dest, jxx::lang::jint destPos,
                          jxx::lang::jint length) {
        if (length <= 0) return;
        std::memmove(dest + destPos, src + srcPos, (size_t)length * sizeof(jxx::lang::jchar));
    }
};
}}
