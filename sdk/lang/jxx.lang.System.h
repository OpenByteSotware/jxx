
#pragma once
#include <memory>
#include <vector>
#include <cstring>
#include <chrono>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::io { class Console; }
namespace jxx::util { template <typename K, typename V> class Map; }
namespace jxx::nio::channels { class Channel; }

namespace jxx { namespace util { class Properties; } namespace lang {
class Object;
class SecurityManager;
class String;
struct System {
    static std::shared_ptr<jxx::io::InputStream> in;
    static std::shared_ptr<jxx::io::PrintStream> out;
    static std::shared_ptr<jxx::io::PrintStream> err;

    static void init();
    static void setIn(const jxx::Ptr<jxx::io::InputStream>& stream);
    static void setOut(const jxx::Ptr<jxx::io::PrintStream>& stream);
    static void setErr(const jxx::Ptr<jxx::io::PrintStream>& stream);

    // Java-like utilities
    static jxx::lang::jlong currentTimeMillis();
    static jxx::lang::jlong nanoTime();
    static jxx::lang::jint identityHashCode(
        const jxx::Ptr<jxx::lang::Object>& object) noexcept;
    static jxx::Ptr<jxx::lang::String> lineSeparator();
    static jxx::Ptr<jxx::io::Console> console();
    static jxx::Ptr<jxx::nio::channels::Channel> inheritedChannel();
    static jxx::Ptr<jxx::lang::String> getenv(
        const jxx::Ptr<jxx::lang::String>& name);
    static jxx::Ptr<jxx::util::Map<jxx::lang::String, jxx::lang::String>> getenv();
    static jxx::Ptr<jxx::lang::String> mapLibraryName(
        const jxx::Ptr<jxx::lang::String>& libraryName);
    static void load(const jxx::Ptr<jxx::lang::String>& filename);
    static void loadLibrary(const jxx::Ptr<jxx::lang::String>& libraryName);
    static void gc();
    static void runFinalization();
    static void runFinalizersOnExit(::jxx::lang::jbool value);
    static void exit(jxx::lang::jint status);
    static jxx::Ptr<jxx::lang::SecurityManager> getSecurityManager();
    static void setSecurityManager(const jxx::Ptr<jxx::lang::SecurityManager>& manager);

    static jxx::Ptr<jxx::util::Properties> getProperties();
    static void setProperties(
        const jxx::Ptr<jxx::util::Properties>& properties);

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
