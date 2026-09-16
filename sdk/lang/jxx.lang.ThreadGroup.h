#pragma once

#include <mutex>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class Thread;
class Throwable;

class ThreadGroup : public ClassBase<ThreadGroup, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<ThreadGroup, JxxSuper>;

    explicit ThreadGroup(const jxx::Ptr<String>& name);
    ThreadGroup(const jxx::Ptr<ThreadGroup>& parent,
                const jxx::Ptr<String>& name);
    ~ThreadGroup() override;

    jxx::Ptr<String> getName() const;
    jxx::Ptr<ThreadGroup> getParent() const;
    jint getMaxPriority() const;
    void setMaxPriority(jint priority);
    jbool isDaemon() const;
    void setDaemon(jbool daemon);
    jbool isDestroyed() const;
    jbool parentOf(const jxx::Ptr<ThreadGroup>& group) const;
    void checkAccess() const;
    jint activeCount() const;
    jint activeGroupCount() const;
    jint enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<Thread>, 1>>& threads) const;
    jint enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<Thread>, 1>>& threads,
                   jbool recurse) const;
    jint enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<ThreadGroup>, 1>>& groups) const;
    jint enumerate(const jxx::Ptr<JxxArray<jxx::Ptr<ThreadGroup>, 1>>& groups,
                   jbool recurse) const;
    void interrupt();
    void destroy();
    void list() const;
    void list_(jint indent) const;
    jbool allowThreadSuspension(jbool value);
    void uncaughtException(const jxx::Ptr<Thread>& thread,
                           const jxx::Ptr<Throwable>& throwable);
    jxx::Ptr<String> toString() const override;

    static jxx::Ptr<ThreadGroup> systemThreadGroup();

private:
    ThreadGroup();
    friend class Thread;
    void addThread_(Thread* thread);
    void removeThread_(Thread* thread);
    void addGroup_(ThreadGroup* group);
    void removeGroup_(ThreadGroup* group);

    mutable std::recursive_mutex mutex_;
    jxx::Ptr<ThreadGroup> parent_;
    jxx::Ptr<String> name_;
    jint maxPriority_ = 10;
    jbool daemon_ = false;
    jbool destroyed_ = false;
    std::vector<Thread*> threads_;
    std::vector<ThreadGroup*> groups_;
};
} // namespace jxx::lang
