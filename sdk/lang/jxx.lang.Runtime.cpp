#include "lang/jxx.lang.Runtime.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <new>
#include <string>
#include <thread>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "io/jxx.io.File.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.SecurityManager.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.UnsatisfiedLinkError.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::lang {

jbool Runtime::runFinalizersOnExit_ = false;
namespace {
std::string platformLibraryName(const std::string& name) {
#ifdef _WIN32
    return name + ".dll";
#elif defined(__APPLE__)
    return "lib" + name + ".dylib";
#else
    return "lib" + name + ".so";
#endif
}
void openLibrary(const std::string& filename) {
#ifdef _WIN32
    if (::LoadLibraryW(std::filesystem::u8path(filename).c_str()) == nullptr) {
        throw UnsatisfiedLinkError(jxx::NEW<String>(filename));
    }
#else
    if (::dlopen(filename.c_str(), RTLD_NOW | RTLD_LOCAL) == nullptr) {
        throw UnsatisfiedLinkError(jxx::NEW<String>(filename));
    }
#endif
}
}

Runtime::Runtime(ConstructionToken)
    : Super() {
}

jxx::Ptr<Runtime> Runtime::getRuntime() {
    static const auto value = jxx::NEW<Runtime>(ConstructionToken{});
    return value;
}

void Runtime::addShutdownHook(const jxx::Ptr<Thread>& hook) {
    if (hook == nullptr) throw NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    if (shuttingDown_) throw IllegalStateException();
    if (hook->isAlive() || hook->getState() != Thread::State::NEW) {
        throw IllegalArgumentException();
    }
    if (std::find(shutdownHooks_.begin(), shutdownHooks_.end(), hook) != shutdownHooks_.end()) {
        throw IllegalArgumentException();
    }
    shutdownHooks_.push_back(hook);
}

jbool Runtime::removeShutdownHook(const jxx::Ptr<Thread>& hook) {
    if (hook == nullptr) throw NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    if (shuttingDown_) throw IllegalStateException();
    const auto found = std::find(shutdownHooks_.begin(), shutdownHooks_.end(), hook);
    if (found == shutdownHooks_.end()) return false;
    shutdownHooks_.erase(found);
    return true;
}

void Runtime::runShutdownHooks_() {
    std::vector<jxx::Ptr<Thread>> hooks;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (shuttingDown_) {
            shutdownCondition_.wait(lock, [this] { return shutdownComplete_; });
            return;
        }
        shuttingDown_ = true;
        hooks.swap(shutdownHooks_);
    }

    // Start every registered hook before waiting for any one hook. Shutdown
    // hooks are independent, and one failed start must not suppress others.
    for (const auto& hook : hooks) {
        try {
            hook->start();
        }
        catch (...) {
        }
    }

    for (const auto& hook : hooks) {
        for (;;) {
            try {
                hook->join();
                break;
            }
            catch (const InterruptedException&) {
                // Shutdown waits for hooks even if the coordinating thread is
                // interrupted.
            }
            catch (...) {
                break;
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        shutdownComplete_ = true;
    }
    shutdownCondition_.notify_all();
}

void Runtime::exit(jint status) {
    const auto manager = System::getSecurityManager();
    if (manager != nullptr) manager->checkExit(status);
    runShutdownHooks_();
    if (runFinalizersOnExit_) runFinalization();
    std::exit(status);
}

void Runtime::halt(jint status) {
    const auto manager = System::getSecurityManager();
    if (manager != nullptr) manager->checkExit(status);
    std::_Exit(status);
}

jint Runtime::availableProcessors() const {
    const auto count = std::thread::hardware_concurrency();
    return static_cast<jint>(count == 0 ? 1 : count);
}

jlong Runtime::totalMemory() const {
#ifdef _WIN32
    MEMORYSTATUSEX value{}; value.dwLength = sizeof(value);
    return ::GlobalMemoryStatusEx(&value) ? static_cast<jlong>(value.ullTotalPhys) : 0;
#else
    const long pages = ::sysconf(_SC_PHYS_PAGES);
    const long size = ::sysconf(_SC_PAGE_SIZE);
    return pages > 0 && size > 0 ? static_cast<jlong>(pages) * static_cast<jlong>(size) : 0;
#endif
}

jlong Runtime::freeMemory() const {
#ifdef _WIN32
    MEMORYSTATUSEX value{}; value.dwLength = sizeof(value);
    return ::GlobalMemoryStatusEx(&value) ? static_cast<jlong>(value.ullAvailPhys) : 0;
#else
    const long pages = ::sysconf(_SC_AVPHYS_PAGES);
    const long size = ::sysconf(_SC_PAGE_SIZE);
    return pages > 0 && size > 0 ? static_cast<jlong>(pages) * static_cast<jlong>(size) : 0;
#endif
}

jlong Runtime::maxMemory() const { return totalMemory(); }
void Runtime::gc() { }
void Runtime::runFinalization() { }

void Runtime::runFinalizersOnExit(::jxx::lang::jbool value) {
    const auto manager = System::getSecurityManager();
    if (manager != nullptr) manager->checkExit(0);
    runFinalizersOnExit_ = value;
}

void Runtime::traceInstructions(jbool) { }
void Runtime::traceMethodCalls(jbool) { }

void Runtime::load(const jxx::Ptr<String>& filename) {
    if (filename == nullptr) throw NullPointerException();
    const auto manager = System::getSecurityManager();
    if (manager != nullptr) manager->checkLink(filename);
    const auto path = std::filesystem::u8path(filename->utf8());
    if (!path.is_absolute()) throw UnsatisfiedLinkError(filename);
    openLibrary(filename->utf8());
}
void Runtime::loadLibrary(const jxx::Ptr<String>& libraryName) {
    if (libraryName == nullptr) throw NullPointerException();
    const auto manager = System::getSecurityManager();
    if (manager != nullptr) manager->checkLink(libraryName);
    const auto name = libraryName->utf8();
    if (name.find('/') != std::string::npos || name.find('\\') != std::string::npos) {
        throw UnsatisfiedLinkError(libraryName);
    }
    openLibrary(platformLibraryName(name));
}

jxx::Ptr<jxx::io::InputStream> Runtime::getLocalizedInputStream(
    const jxx::Ptr<jxx::io::InputStream>& input) { return input; }
jxx::Ptr<jxx::io::OutputStream> Runtime::getLocalizedOutputStream(
    const jxx::Ptr<jxx::io::OutputStream>& output) { return output; }

namespace {
jxx::Ptr<JxxArray<jxx::Ptr<String>,1>> splitCommand(const jxx::Ptr<String>& command) {
    if (command == nullptr) throw NullPointerException();
    std::istringstream input(command->utf8());
    std::vector<std::string> parts; std::string part;
    while (input >> part) parts.push_back(part);
    if (parts.empty()) throw IllegalArgumentException();
    auto result=jxx::NEW<JxxArray<jxx::Ptr<String>,1>>(parts.size());
    for(std::uint32_t i=0;i<result->length;++i)(*result)[i]=jxx::NEW<String>(parts[i]);
    return result;
}
void applyEnvironment(const jxx::Ptr<ProcessBuilder>& builder,
 const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& environment) {
    if (environment == nullptr) return;
    builder->clearEnvironment();
    for(std::uint32_t i=0;i<environment->length;++i){
        const auto& entry=(*environment)[i]; if(entry==nullptr)throw NullPointerException();
        auto text=entry->utf8(); auto position=text.find('=');
        if(position==std::string::npos||position==0)throw IllegalArgumentException(entry);
        builder->environment(jxx::NEW<String>(text.substr(0,position)),jxx::NEW<String>(text.substr(position+1)));
    }
}
}
jxx::Ptr<Process> Runtime::exec(const jxx::Ptr<String>& command){return exec(command,nullptr,nullptr);}
jxx::Ptr<Process> Runtime::exec(const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& command){return exec(command,nullptr,nullptr);}
jxx::Ptr<Process> Runtime::exec(const jxx::Ptr<String>& command,const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& environment){return exec(command,environment,nullptr);}
jxx::Ptr<Process> Runtime::exec(const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& command,const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& environment){return exec(command,environment,nullptr);}
jxx::Ptr<Process> Runtime::exec(const jxx::Ptr<String>& command,const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& environment,const jxx::Ptr<jxx::io::File>& directory){return exec(splitCommand(command),environment,directory);}
jxx::Ptr<Process> Runtime::exec(const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& command,const jxx::Ptr<JxxArray<jxx::Ptr<String>,1>>& environment,const jxx::Ptr<jxx::io::File>& directory){auto builder=jxx::NEW<ProcessBuilder>(command);applyEnvironment(builder,environment);builder->directory(directory);return builder->start();}

} // namespace jxx::lang
