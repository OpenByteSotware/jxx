#include "lang/jxx.lang.ThreadGroup.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.SecurityManager.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {
namespace {
template <typename T> void erasePointer(std::vector<T*>& values, T* value) {
    values.erase(std::remove(values.begin(), values.end(), value), values.end());
}
}
ThreadGroup::ThreadGroup(const jxx::Ptr<String>& name)
    : ThreadGroup(systemThreadGroup(), name) {}
ThreadGroup::ThreadGroup(const jxx::Ptr<ThreadGroup>& parent,
                         const jxx::Ptr<String>& name)
    : Super(), parent_(parent), name_(name) {
    if (parent_ == nullptr || name_ == nullptr) throw NullPointerException();
    parent_->checkAccess();
    maxPriority_ = parent_->getMaxPriority();
    parent_->addGroup_(this);
}
ThreadGroup::~ThreadGroup() {
    if (parent_ != nullptr) parent_->removeGroup_(this);
}
jxx::Ptr<ThreadGroup> ThreadGroup::systemThreadGroup() {
    static auto value = [] {
        auto group = jxx::Ptr<ThreadGroup>(new ThreadGroup());
        group->name_ = jxx::NEW<String>("system");
        group->maxPriority_ = Thread::MAX_PRIORITY;
        return group;
    }();
    return value;
}
ThreadGroup::ThreadGroup() : Super() {}
jxx::Ptr<String> ThreadGroup::getName() const { return name_; }
jxx::Ptr<ThreadGroup> ThreadGroup::getParent() const { checkAccess(); return parent_; }
jint ThreadGroup::getMaxPriority() const { std::lock_guard<std::recursive_mutex> l(mutex_); return maxPriority_; }
void ThreadGroup::setMaxPriority(jint p) {
    checkAccess();
    if (p < Thread::MIN_PRIORITY || p > Thread::MAX_PRIORITY) return;
    std::lock_guard<std::recursive_mutex> l(mutex_);
    maxPriority_ = parent_ == nullptr ? p : std::min(p, parent_->getMaxPriority());
    for (auto* group : groups_) if (group != nullptr) group->setMaxPriority(maxPriority_);
}
jbool ThreadGroup::isDaemon() const { std::lock_guard<std::recursive_mutex> l(mutex_); return daemon_; }
void ThreadGroup::setDaemon(jbool v) { checkAccess(); std::lock_guard<std::recursive_mutex> l(mutex_); daemon_ = v; }
jbool ThreadGroup::isDestroyed() const { std::lock_guard<std::recursive_mutex> l(mutex_); return destroyed_; }
jbool ThreadGroup::parentOf(const jxx::Ptr<ThreadGroup>& g) const {
    for (auto current = g; current != nullptr; current = current->parent_) if (current.get() == this) return true;
    return false;
}
void ThreadGroup::checkAccess() const {
    const auto manager = System::getSecurityManager();
    if (manager != nullptr) {
        const auto self = jxx::CAST<ThreadGroup>(
            const_cast<ThreadGroup*>(this)->thisPtr());
        manager->checkAccess(self);
    }
}
jint ThreadGroup::activeCount() const {
    std::lock_guard<std::recursive_mutex> l(mutex_); jint n = 0;
    for (auto* t : threads_) if (t != nullptr && t->isAlive()) ++n;
    for (auto* g : groups_) if (g != nullptr) n += g->activeCount();
    return n;
}
jint ThreadGroup::activeGroupCount() const {
    std::lock_guard<std::recursive_mutex> l(mutex_); jint n = 0;
    for (auto* g : groups_) if (g != nullptr && !g->isDestroyed()) { ++n; n += g->activeGroupCount(); }
    return n;
}
jint ThreadGroup::enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<Thread>,1>>& a) const { return enumerate(a, true); }
jint ThreadGroup::enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<Thread>,1>>& a, jbool recurse) const {
    if (a == nullptr) throw NullPointerException();
    std::lock_guard<std::recursive_mutex> l(mutex_); jint n = 0;
    for (auto* t : threads_) if (t && t->isAlive() && n < static_cast<jint>(a->length)) (*a)[n++] = jxx::CAST<Thread>(t->thisPtr());
    if (recurse) for (auto* g : groups_) if (g && n < static_cast<jint>(a->length)) {
        auto remaining = jxx::NEW<JxxArray<jxx::Ptr<Thread>,1>>(a->length - n);
        jint added = g->enumerate(remaining, true);
        for (jint i=0;i<added;++i) (*a)[n+i]=(*remaining)[i]; n += added;
    }
    return n;
}
jint ThreadGroup::enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<ThreadGroup>,1>>& a) const { return enumerate(a, true); }
jint ThreadGroup::enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<ThreadGroup>,1>>& a, jbool recurse) const {
    if (a == nullptr) throw NullPointerException();
    std::lock_guard<std::recursive_mutex> l(mutex_); jint n = 0;
    for (auto* g : groups_) if (g && !g->isDestroyed() && n < static_cast<jint>(a->length)) {
        (*a)[n++] = jxx::CAST<ThreadGroup>(g->thisPtr());
        if (recurse && n < static_cast<jint>(a->length)) {
            auto remaining=jxx::NEW<JxxArray<jxx::Ptr<ThreadGroup>,1>>(a->length-n);
            jint added=g->enumerate(remaining,true); for(jint i=0;i<added;++i)(*a)[n+i]=(*remaining)[i]; n+=added;
        }
    }
    return n;
}
void ThreadGroup::interrupt() { std::lock_guard<std::recursive_mutex> l(mutex_); for(auto* t:threads_)if(t)t->interrupt(); for(auto* g:groups_)if(g)g->interrupt(); }
void ThreadGroup::stop() {
    checkAccess();
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    for (auto* thread : threads_) if (thread != nullptr) thread->stop();
    for (auto* group : groups_) if (group != nullptr) group->stop();
}
void ThreadGroup::suspend() {
    checkAccess();
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    for (auto* thread : threads_) if (thread != nullptr) thread->suspend();
    for (auto* group : groups_) if (group != nullptr) group->suspend();
}
void ThreadGroup::resume() {
    checkAccess();
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    for (auto* thread : threads_) if (thread != nullptr) thread->resume();
    for (auto* group : groups_) if (group != nullptr) group->resume();
}
void ThreadGroup::destroy() {
    checkAccess(); std::lock_guard<std::recursive_mutex> l(mutex_);
    if (destroyed_ || !threads_.empty()) throw IllegalThreadStateException();
    for (auto* g : groups_) if (g) g->destroy();
    groups_.clear(); destroyed_ = true;
    if (parent_) parent_->removeGroup_(this);
}
void ThreadGroup::list() const { list_(0); }
void ThreadGroup::list_(jint indent) const {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    std::cout << std::string(static_cast<std::size_t>(indent), ' ')
              << toString()->utf8() << std::endl;
    for (auto* thread : threads_) {
        if (thread != nullptr && thread->isAlive()) {
            std::cout << std::string(static_cast<std::size_t>(indent + 4), ' ')
                      << thread->toString()->utf8() << std::endl;
        }
    }
    for (auto* group : groups_) {
        if (group != nullptr && !group->isDestroyed()) group->list_(indent + 4);
    }
}
jbool ThreadGroup::allowThreadSuspension(jbool) { return true; }
void ThreadGroup::uncaughtException(
    const jxx::Ptr<Thread>& thread,
    const jxx::Ptr<Throwable>& throwable) {
    if (throwable == nullptr) return;
    if (parent_ != nullptr) {
        parent_->uncaughtException(thread, throwable);
        return;
    }
    const auto defaultHandler =
        Thread::getDefaultUncaughtExceptionHandler();
    if (defaultHandler != nullptr &&
        defaultHandler.get() != this) {
        defaultHandler->uncaughtException(thread, throwable);
        return;
    }
    std::cerr << "Exception in thread "
              << (thread == nullptr ? std::string("unknown") : thread->getName()->utf8())
              << ": " << throwable->toString()->utf8() << std::endl;
    throwable->printStackTrace(std::cerr);
}
jxx::Ptr<String> ThreadGroup::toString() const { std::ostringstream s; s << "ThreadGroup[name=" << name_->utf8() << ",maxpri=" << getMaxPriority() << ']'; return jxx::NEW<String>(s.str()); }
void ThreadGroup::addThread_(Thread* t) { std::lock_guard<std::recursive_mutex> l(mutex_); if(destroyed_)throw IllegalThreadStateException(); threads_.push_back(t); }
void ThreadGroup::removeThread_(Thread* thread) {
    jbool autoDestroy = false;
    {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        erasePointer(threads_, thread);
        autoDestroy = daemon_ && !destroyed_ && threads_.empty() && groups_.empty() && parent_ != nullptr;
        if (autoDestroy) destroyed_ = true;
    }
    if (autoDestroy) parent_->removeGroup_(this);
}
void ThreadGroup::addGroup_(ThreadGroup* g) { std::lock_guard<std::recursive_mutex> l(mutex_); if(destroyed_)throw IllegalThreadStateException(); groups_.push_back(g); }
void ThreadGroup::removeGroup_(ThreadGroup* group) {
    jbool autoDestroy = false;
    {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        erasePointer(groups_, group);
        autoDestroy = daemon_ && !destroyed_ && threads_.empty() && groups_.empty() && parent_ != nullptr;
        if (autoDestroy) destroyed_ = true;
    }
    if (autoDestroy) parent_->removeGroup_(this);
}
} // namespace jxx::lang
