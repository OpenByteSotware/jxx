#include <gtest/gtest.h>

#include <vector>

#include "lang/jxx.lang.Runtime.h"
#include "lang/jxx.lang.SecurityManager.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.Thread.h"
#include "security/jxx.security.Permission.h"

namespace {
using ::jxx::lang::Runtime;
using ::jxx::lang::SecurityManager;
using ::jxx::lang::String;
using ::jxx::lang::System;
using ::jxx::lang::Thread;

class RecordingSecurityManager final : public SecurityManager {
public:
    void checkPermission(
        const ::jxx::Ptr<::jxx::security::Permission>& permission) override {
        if (permission != nullptr && permission->getName() != nullptr) {
            names.push_back(permission->getName()->utf8());
        }
    }

    void checkPropertiesAccess() override {
        ++propertiesChecks;
    }

    void checkPropertyAccess(const ::jxx::Ptr<String>& key) override {
        checkedProperty = key == nullptr ? std::string() : key->utf8();
    }

    std::vector<std::string> names;
    int propertiesChecks = 0;
    std::string checkedProperty;
};

TEST(SystemSecurityIntegrationTest, EnvironmentAccessUsesRuntimePermissions) {
    const auto manager = ::jxx::NEW<RecordingSecurityManager>();
    System::setSecurityManager(manager);

    const auto path = ::jxx::NEW<String>("PATH");
    (void)System::getenv(path);
    (void)System::getenv();

    EXPECT_NE(manager->names.end(),
              std::find(manager->names.begin(), manager->names.end(), "getenv.PATH"));
    EXPECT_NE(manager->names.end(),
              std::find(manager->names.begin(), manager->names.end(), "getenv.*"));
    System::setSecurityManager(nullptr);
}

TEST(SystemSecurityIntegrationTest, PropertyOperationsInvokeSecurityChecks) {
    const auto manager = ::jxx::NEW<RecordingSecurityManager>();
    System::setSecurityManager(manager);
    const auto key = ::jxx::NEW<String>("jxx.test.secured-property");

    (void)System::getProperty(key);
    EXPECT_EQ("jxx.test.secured-property", manager->checkedProperty);
    (void)System::setProperty(key, ::jxx::NEW<String>("value"));
    (void)System::clearProperty(key);
    EXPECT_GE(manager->propertiesChecks, 2);
    System::setSecurityManager(nullptr);
}

TEST(RuntimeSecurityIntegrationTest, ShutdownHookMutationChecksPermission) {
    const auto manager = ::jxx::NEW<RecordingSecurityManager>();
    System::setSecurityManager(manager);
    const auto hook = ::jxx::NEW<Thread>();

    Runtime::getRuntime()->addShutdownHook(hook);
    EXPECT_TRUE(Runtime::getRuntime()->removeShutdownHook(hook));
    EXPECT_NE(manager->names.end(),
              std::find(manager->names.begin(), manager->names.end(), "shutdownHooks"));
    System::setSecurityManager(nullptr);
}

} // namespace
