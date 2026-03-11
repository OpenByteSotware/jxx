#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <cstring>
#include <chrono>

#if defined(_WIN32)
#include <windows.h>
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <dlfcn.h>
#include <unistd.h>
extern char **environ;
#endif

#include "lang/jxx.lang.System.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.InputStream.h"
#include "util/jxx.util.Properties.h"
#include "io/jxx.io.Console.h"

namespace jxx { namespace lang {

static jxx::io::PrintStream* g_out = nullptr;
static jxx::io::PrintStream* g_err = nullptr;
static jxx::io::InputStream* g_in = nullptr;

static jxx::util::Properties g_props;
static bool g_props_init = false;
static void ensureProps_() {
    if (g_props_init) return;
    g_props_init = true;
    // Minimal default properties
    g_props.setProperty("line.separator", System::lineSeparator());
    g_props.setProperty("file.separator", System::fileSeparator());
#ifdef _WIN32
    g_props.setProperty("os.name", "Windows");
#else
    g_props.setProperty("os.name", "Unix");
#endif
}

jxx::io::PrintStream& System::out() {
    if (!g_out) g_out = new jxx::io::PrintStream(std::cout);
    return *g_out;
}

jxx::io::PrintStream& System::err() {
    if (!g_err) g_err = new jxx::io::PrintStream(std::cerr);
    return *g_err;
}

jxx::io::InputStream& System::in() {
    if (!g_in) g_in = new jxx::io::StdInputStream(std::cin);
    return *g_in;
}

void System::setIn(jxx::io::InputStream* is) { g_in = is; }
void System::setOut(jxx::io::PrintStream* ps) { g_out = ps; }
void System::setErr(jxx::io::PrintStream* ps) { g_err = ps; }

std::int64_t System::currentTimeMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

std::int64_t System::nanoTime() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
}

const char* System::getenv(const std::string& key) {
    return std::getenv(key.c_str());
}

std::unordered_map<std::string,std::string> System::getenvMap() {
    std::unordered_map<std::string,std::string> m;
#if defined(_WIN32)
    char** envp = *_environ;
    for (char** e = envp; *e; ++e) {
        std::string s(*e);
        auto pos = s.find('=');
        if (pos!=std::string::npos) m.emplace(s.substr(0,pos), s.substr(pos+1));
    }
#else
    for (char** e = environ; *e; ++e) {
        std::string s(*e);
        auto pos = s.find('=');
        if (pos!=std::string::npos) m.emplace(s.substr(0,pos), s.substr(pos+1));
    }
#endif
    return m;
}

std::string System::lineSeparator() { return "\n"; }
std::string System::fileSeparator() {
#ifdef _WIN32
    return "\\";
#else
    return "/";
#endif
}

jxx::util::Properties& System::getProperties() { ensureProps_(); return g_props; }
void System::setProperties(const jxx::util::Properties& p) { g_props = p; g_props_init = true; }
std::string System::getProperty(const std::string& key) { ensureProps_(); return g_props.getProperty(key); }
std::string System::getProperty(const std::string& key, const std::string& def) { ensureProps_(); return g_props.getProperty(key, def); }
std::string System::setProperty(const std::string& key, const std::string& value) { ensureProps_(); return g_props.setProperty(key, value); }
std::string System::clearProperty(const std::string& key) { ensureProps_(); return g_props.clearProperty(key); }

std::string System::mapLibraryName(const std::string& libname) {
#ifdef _WIN32
    return libname + ".dll";
#elif defined(__APPLE__)
    return std::string("lib") + libname + ".dylib";
#else
    return std::string("lib") + libname + ".so";
#endif
}

void System::load(const std::string& filename) {
#ifdef _WIN32
    HMODULE h = LoadLibraryA(filename.c_str());
    if (!h) throw std::runtime_error("System.load failed");
#else
    void* h = dlopen(filename.c_str(), RTLD_NOW);
    if (!h) throw std::runtime_error(std::string("System.load failed: ") + dlerror());
#endif
}

void System::loadLibrary(const std::string& libname) {
    std::string mapped = mapLibraryName(libname);
#ifdef _WIN32
    HMODULE h = LoadLibraryA(mapped.c_str());
    if (!h) throw std::runtime_error("System.loadLibrary failed");
#else
    void* h = dlopen(mapped.c_str(), RTLD_NOW);
    if (!h) throw std::runtime_error(std::string("System.loadLibrary failed: ") + dlerror());
#endif
}

jxx::io::Console* System::console() {
    if (isatty(fileno(stdin)) && isatty(fileno(stdout))) return new jxx::io::Console();
    return nullptr;
}

void System::gc() { /* hint only; no GC in C++ */ }
void System::runFinalization() { /* finalizers not modeled */ }
void System::exit(int status) { std::exit(status); }

}} // namespace jxx::lang
