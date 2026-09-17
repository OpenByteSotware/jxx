#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::security { class Permission; }
namespace jxx::lang {
class ClassAny;
class String;
class Thread;
class ThreadGroup;

class SecurityManager
    : public ClassBase<SecurityManager, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<SecurityManager, JxxSuper>;
    using JxxClassInfoMarker = ClassInfo<SecurityManager, JxxSuper>;

    static jxx::Ptr<ClassAny> Class();
    SecurityManager();
    ~SecurityManager() override = default;

    virtual void checkPermission(const jxx::Ptr<jxx::security::Permission>& permission);
    virtual void checkPermission(const jxx::Ptr<jxx::security::Permission>& permission,
        const jxx::Ptr<Object>& context);
    virtual jxx::Ptr<Object> getSecurityContext();
    virtual void checkCreateClassLoader();
    virtual void checkAccess(const jxx::Ptr<Thread>& thread);
    virtual void checkAccess(const jxx::Ptr<ThreadGroup>& group);
    virtual void checkExit(jint status);
    virtual void checkExec(const jxx::Ptr<String>& command);
    virtual void checkLink(const jxx::Ptr<String>& library);
    virtual void checkRead(const jxx::Ptr<String>& file);
    virtual void checkRead(const jxx::Ptr<String>& file, const jxx::Ptr<Object>& context);
    virtual void checkWrite(const jxx::Ptr<String>& file);
    virtual void checkDelete(const jxx::Ptr<String>& file);
    virtual void checkConnect(const jxx::Ptr<String>& host, jint port);
    virtual void checkConnect(const jxx::Ptr<String>& host, jint port,
        const jxx::Ptr<Object>& context);
    virtual void checkListen(jint port);
    virtual void checkAccept(const jxx::Ptr<String>& host, jint port);
    virtual void checkMulticast(const jxx::Ptr<Object>& address);
    virtual void checkPropertiesAccess();
    virtual void checkPropertyAccess(const jxx::Ptr<String>& key);
    virtual void checkPrintJobAccess();
    virtual void checkPackageAccess(const jxx::Ptr<String>& packageName);
    virtual void checkPackageDefinition(const jxx::Ptr<String>& packageName);
    virtual void checkSetFactory();
    virtual void checkSecurityAccess(const jxx::Ptr<String>& target);
    virtual jbool checkTopLevelWindow(const jxx::Ptr<Object>& window);

protected:
    virtual jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> getClassContext();
    virtual jxx::Ptr<ClassAny> currentLoadedClass();
    virtual jint classDepth(const jxx::Ptr<String>& name);
    virtual jint classLoaderDepth();
    virtual jbool inClass(const jxx::Ptr<String>& name);
    virtual jbool inClassLoader();
};
} // namespace jxx::lang
