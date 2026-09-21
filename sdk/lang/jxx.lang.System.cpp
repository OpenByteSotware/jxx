#include "io/jxx.io.FileDescriptor.h"
#include "lang/jxx.lang.System.h"

#include <cstdlib>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runtime.h"
#include "lang/jxx.lang.SecurityManager.h"
#include "lang/jxx.lang.RuntimePermission.h"
#include "util/jxx.util.Properties.h"

using namespace jxx::io;
namespace jxx { namespace lang {


namespace {
jxx::Ptr<SecurityManager>& securityManager() { static jxx::Ptr<SecurityManager> value; return value; }
std::mutex& securityManagerMutex() { static std::mutex value; return value; }

std::mutex& propertyMutex() {
    static std::mutex mutex;
    return mutex;
}

jxx::Ptr<jxx::util::Properties>& systemProperties() {
    static auto values = jxx::NEW<jxx::util::Properties>();
    return values;
}

void requirePropertyKey(const jxx::Ptr<String>& key) {
    if (key == nullptr) throw NullPointerException();
    if (key->length() == 0) throw IllegalArgumentException();
}
} // namespace

std::shared_ptr<InputStream> System::in;
std::shared_ptr<PrintStream> System::out;
std::shared_ptr<PrintStream> System::err;

static struct __SystemBootstrap { __SystemBootstrap(){ System::init(); } } __sys_bootstrap;

void System::init(){
    System::in  = jxx::NEW<FileInputStream>(FileDescriptor::in);
    auto outOs  = jxx::NEW<FileOutputStream>(FileDescriptor::out);
    auto errOs  = jxx::NEW<FileOutputStream>(FileDescriptor::err);
    System::out = jxx::NEW<PrintStream>(outOs, true);
    System::err = jxx::NEW<PrintStream>(errOs, true);
}

jxx::lang::jlong System::currentTimeMillis() {
    using namespace std::chrono;
    auto now = time_point_cast<milliseconds>(system_clock::now());
    return (jxx::lang::jlong)now.time_since_epoch().count();
}

jxx::lang::jlong System::nanoTime() {
    using namespace std::chrono;
    return static_cast<jxx::lang::jlong>(
        duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
}

jxx::lang::jint System::identityHashCode(
    const jxx::Ptr<jxx::lang::Object>& object) noexcept {
    if (object == nullptr) {
        return 0;
    }
    const auto address = reinterpret_cast<std::uintptr_t>(object.get());
    return static_cast<jxx::lang::jint>(
        address ^ (address >> 32));
}

jxx::Ptr<String> System::lineSeparator() {
#ifdef _WIN32
    static const auto value = jxx::NEW<String>("\r\n");
#else
    static const auto value = jxx::NEW<String>("\n");
#endif
    return value;
}

jxx::Ptr<String> System::getenv(const jxx::Ptr<String>& name) {
    if (name == nullptr) throw NullPointerException();
    if (name->length() == 0) throw IllegalArgumentException();
    const char* value = std::getenv(name->utf8().c_str());
    return value == nullptr ? nullptr : jxx::NEW<String>(value);
}

void System::load(const jxx::Ptr<String>& filename) {
    Runtime::getRuntime()->load(filename);
}

void System::loadLibrary(const jxx::Ptr<String>& libraryName) {
    Runtime::getRuntime()->loadLibrary(libraryName);
}

jxx::Ptr<String> System::mapLibraryName(
    const jxx::Ptr<String>& libraryName) {
    if (libraryName == nullptr) {
        throw NullPointerException();
    }
#ifdef _WIN32
    return jxx::NEW<String>(libraryName->utf8() + ".dll");
#elif defined(__APPLE__)
    return jxx::NEW<String>("lib" + libraryName->utf8() + ".dylib");
#else
    return jxx::NEW<String>("lib" + libraryName->utf8() + ".so");
#endif
}

void System::gc() {
    Runtime::getRuntime()->gc();
}

void System::runFinalization() {
    Runtime::getRuntime()->runFinalization();
}

void System::exit(jxx::lang::jint status) {
    Runtime::getRuntime()->exit(status);
}

jxx::Ptr<jxx::util::Properties> System::getProperties() {
    std::lock_guard<std::mutex> guard(propertyMutex());
    return systemProperties();
}

void System::setProperties(
    const jxx::Ptr<jxx::util::Properties>& values) {
    std::lock_guard<std::mutex> guard(propertyMutex());
    systemProperties() = values == nullptr
        ? jxx::NEW<jxx::util::Properties>()
        : values;
}

jxx::Ptr<String> System::getProperty(const jxx::Ptr<String>& key) {
    requirePropertyKey(key);
    std::lock_guard<std::mutex> guard(propertyMutex());
    return systemProperties()->getProperty(key);
}

jxx::Ptr<String> System::getProperty(
    const jxx::Ptr<String>& key,
    const jxx::Ptr<String>& defaultValue) {
    requirePropertyKey(key);
    std::lock_guard<std::mutex> guard(propertyMutex());
    return systemProperties()->getProperty(key, defaultValue);
}

jxx::Ptr<String> System::setProperty(
    const jxx::Ptr<String>& key,
    const jxx::Ptr<String>& value) {
    requirePropertyKey(key);
    if (value == nullptr) throw NullPointerException();
    std::lock_guard<std::mutex> guard(propertyMutex());
    return jxx::CAST<String>(systemProperties()->setProperty(key, value));
}

jxx::Ptr<String> System::clearProperty(const jxx::Ptr<String>& key) {
    requirePropertyKey(key);
    std::lock_guard<std::mutex> guard(propertyMutex());
    const auto previous = systemProperties()->getProperty(key);
    if (previous != nullptr) {
        systemProperties()->remove(jxx::CAST<Object>(key));
    }
    return previous;
}

jxx::Ptr<SecurityManager> System::getSecurityManager() {
    std::lock_guard<std::mutex> guard(securityManagerMutex());
    return securityManager();
}
void System::setSecurityManager(const jxx::Ptr<SecurityManager>& manager) {
    std::lock_guard<std::mutex> guard(securityManagerMutex());
    if (securityManager() != nullptr) {
        securityManager()->checkPermission(jxx::NEW<RuntimePermission>(jxx::NEW<String>("setSecurityManager")));
    }
    securityManager() = manager;
}

}} // ns

