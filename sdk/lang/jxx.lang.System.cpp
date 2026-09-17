#include "io/jxx.io.FileDescriptor.h"
#include "lang/jxx.lang.System.h"

#include <cstdlib>
#include <mutex>
#include <string>
#include <unordered_map>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

using namespace jxx::io;
namespace jxx { namespace lang {


namespace {
std::mutex& propertyMutex() {
    static std::mutex mutex;
    return mutex;
}

std::unordered_map<std::string, std::string>& properties() {
    static std::unordered_map<std::string, std::string> values;
    return values;
}

void requirePropertyKey(const jxx::Ptr<String>& key) {
    if (key == nullptr || key->length() == 0) {
        throw NullPointerException();
    }
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

jxx::Ptr<String> System::getProperty(const jxx::Ptr<String>& key) {
    requirePropertyKey(key);
    std::lock_guard<std::mutex> guard(propertyMutex());
    const auto found = properties().find(key->utf8());
    if (found != properties().end()) {
        return jxx::NEW<String>(found->second);
    }
    const char* environmentValue = std::getenv(key->utf8().c_str());
    return environmentValue == nullptr
        ? nullptr
        : jxx::NEW<String>(environmentValue);
}

jxx::Ptr<String> System::getProperty(
    const jxx::Ptr<String>& key,
    const jxx::Ptr<String>& defaultValue) {
    const auto value = getProperty(key);
    return value == nullptr ? defaultValue : value;
}

jxx::Ptr<String> System::setProperty(
    const jxx::Ptr<String>& key,
    const jxx::Ptr<String>& value) {
    requirePropertyKey(key);
    if (value == nullptr) {
        throw NullPointerException();
    }
    std::lock_guard<std::mutex> guard(propertyMutex());
    auto& values = properties();
    const auto found = values.find(key->utf8());
    auto previous = found == values.end()
        ? nullptr
        : jxx::NEW<String>(found->second);
    values[key->utf8()] = value->utf8();
    return previous;
}

jxx::Ptr<String> System::clearProperty(const jxx::Ptr<String>& key) {
    requirePropertyKey(key);
    std::lock_guard<std::mutex> guard(propertyMutex());
    auto& values = properties();
    const auto found = values.find(key->utf8());
    if (found == values.end()) {
        return nullptr;
    }
    auto previous = jxx::NEW<String>(found->second);
    values.erase(found);
    return previous;
}

}} // ns

