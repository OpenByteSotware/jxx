#include "lang/jxx.lang.SecurityManager.h"

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.RuntimePermission.h"
#include "lang/jxx.lang.SecurityException.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.Permission.h"

namespace jxx::lang {
namespace {
void requireString(const jxx::Ptr<String>& value) { if (!value) throw NullPointerException(); }
jxx::Ptr<RuntimePermission> permission(const std::string& name) {
    return jxx::NEW<RuntimePermission>(jxx::NEW<String>(name));
}
}
jxx::Ptr<ClassAny> SecurityManager::Class() { return JxxClassInfoMarker::Class(); }
SecurityManager::SecurityManager() : Super() {}
void SecurityManager::checkPermission(const jxx::Ptr<jxx::security::Permission>& value) { if (!value) throw NullPointerException(); }
void SecurityManager::checkPermission(const jxx::Ptr<jxx::security::Permission>& value, const jxx::Ptr<Object>& context) { if (!context) throw SecurityException(); checkPermission(value); }
jxx::Ptr<Object> SecurityManager::getSecurityContext() { return jxx::CAST<Object>(thisPtr()); }
void SecurityManager::checkCreateClassLoader() { checkPermission(permission("createClassLoader")); }
void SecurityManager::checkAccess(const jxx::Ptr<Thread>& value) { if (!value) throw NullPointerException(); checkPermission(permission("modifyThread")); }
void SecurityManager::checkAccess(const jxx::Ptr<ThreadGroup>& value) { if (!value) throw NullPointerException(); checkPermission(permission("modifyThreadGroup")); }
void SecurityManager::checkExit(jint status) { checkPermission(permission("exitVM." + std::to_string(status))); }
void SecurityManager::checkExec(const jxx::Ptr<String>& value) { requireString(value); }
void SecurityManager::checkLink(const jxx::Ptr<String>& value) { requireString(value); checkPermission(permission("loadLibrary." + value->utf8())); }
void SecurityManager::checkRead(const jxx::Ptr<String>& value) { requireString(value); }
void SecurityManager::checkRead(const jxx::Ptr<String>& value, const jxx::Ptr<Object>& context) { requireString(value); if (!context) throw SecurityException(); }
void SecurityManager::checkWrite(const jxx::Ptr<String>& value) { requireString(value); }
void SecurityManager::checkDelete(const jxx::Ptr<String>& value) { requireString(value); }
void SecurityManager::checkConnect(const jxx::Ptr<String>& host, jint) { requireString(host); }
void SecurityManager::checkConnect(const jxx::Ptr<String>& host, jint port, const jxx::Ptr<Object>& context) { requireString(host); if (!context) throw SecurityException(); checkConnect(host, port); }
void SecurityManager::checkListen(jint) {}
void SecurityManager::checkAccept(const jxx::Ptr<String>& host, jint) { requireString(host); }
void SecurityManager::checkMulticast(const jxx::Ptr<Object>& address) { if (!address) throw NullPointerException(); }
void SecurityManager::checkPropertiesAccess() { checkPermission(permission("properties")); }
void SecurityManager::checkPropertyAccess(const jxx::Ptr<String>& key) { requireString(key); checkPermission(permission("getProperty." + key->utf8())); }
void SecurityManager::checkPrintJobAccess() { checkPermission(permission("queuePrintJob")); }
void SecurityManager::checkPackageAccess(const jxx::Ptr<String>& value) { requireString(value); }
void SecurityManager::checkPackageDefinition(const jxx::Ptr<String>& value) { requireString(value); }
void SecurityManager::checkSetFactory() { checkPermission(permission("setFactory")); }
void SecurityManager::checkSecurityAccess(const jxx::Ptr<String>& value) { requireString(value); }
jbool SecurityManager::checkTopLevelWindow(const jxx::Ptr<Object>& value) { return value != nullptr; }
jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> SecurityManager::getClassContext() { return jxx::NEW<JxxArray<jxx::Ptr<ClassAny>, 1>>(0); }
jxx::Ptr<ClassAny> SecurityManager::currentLoadedClass() { return nullptr; }
jint SecurityManager::classDepth(const jxx::Ptr<String>& name) { requireString(name); return -1; }
jint SecurityManager::classLoaderDepth() { return -1; }
jbool SecurityManager::inClass(const jxx::Ptr<String>& name) { return classDepth(name) >= 0; }
jbool SecurityManager::inClassLoader() { return false; }
} // namespace jxx::lang
