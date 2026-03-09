#pragma once
#include "jxx.lang.Object.h"
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <cstring>
#include <cstdint>

namespace jxx { namespace io { class PrintStream; class InputStream; class Console; }}
namespace jxx { namespace util { class Properties; }}

namespace jxx { namespace lang {

class System final : public Object {
public:
    // Streams (setters to match Java System.setOut/Err/In)
    static jxx::io::PrintStream& out();
    static void setOut(jxx::io::PrintStream* ps);

    static jxx::io::PrintStream& err();
    static void setErr(jxx::io::PrintStream* ps);

    static jxx::io::InputStream& in();
    static void setIn(jxx::io::InputStream* is);

    // Time
    static std::int64_t currentTimeMillis();
    static std::int64_t nanoTime();

    // Properties
    static jxx::util::Properties& getProperties();
    static void setProperties(const jxx::util::Properties& p);
    static std::string getProperty(const std::string& key);
    static std::string getProperty(const std::string& key, const std::string& def);
    static std::string setProperty(const std::string& key, const std::string& value);
    static std::string clearProperty(const std::string& key);

    // Environment
    static const char* getenv(const std::string& key);
    static std::unordered_map<std::string, std::string> getenvMap();

    // Identity hash
    static std::size_t identityHashCode(const Object& obj) { return reinterpret_cast<std::size_t>(&obj); }

    // Library loading
    static void load(const std::string& filename);
    static void loadLibrary(const std::string& libname);
    static std::string mapLibraryName(const std::string& libname);

    // Console
    static jxx::io::Console* console(); // returns nullptr if not available

    // GC/Finalization/Exit
    static void gc();
    static void runFinalization();
    static void exit(int status);

    // arraycopy: raw pointers and vectors
    template <typename T>
    static void arraycopy(const T* src, int srcPos, T* dest, int destPos, int length) {
        if (!src || !dest || srcPos < 0 || destPos < 0 || length < 0) throw std::out_of_range("arraycopy: invalid");
        std::memmove(dest + destPos, src + srcPos, sizeof(T) * static_cast<std::size_t>(length));
    }

    template <typename T>
    static void arraycopy(const std::vector<T>& src, int srcPos, std::vector<T>& dest, int destPos, int length) {
        if (srcPos < 0 || destPos < 0 || length < 0) throw std::out_of_range("arraycopy: negative index");
        if (static_cast<std::size_t>(srcPos + length) > src.size()) throw std::out_of_range("arraycopy: src range");
        if (static_cast<std::size_t>(destPos + length) > dest.size()) dest.resize(destPos + length);
        std::memmove(&dest[destPos], &src[srcPos], sizeof(T) * static_cast<std::size_t>(length));
    }

    static std::string lineSeparator();
    static std::string fileSeparator();
};

}} // namespace jxx::lang
