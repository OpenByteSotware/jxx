#pragma once
#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.Map.h"
namespace jxx::lang::management {
class RuntimeMXBean : public ::jxx::lang::InterfaceBase<RuntimeMXBean,PlatformManagedObject>{public:~RuntimeMXBean() override=default;virtual ::jxx::Ptr<::jxx::lang::String> getName() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getVmName() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getVmVendor() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getVmVersion() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getSpecName() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getSpecVendor() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getSpecVersion() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getManagementSpecVersion() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getClassPath() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getLibraryPath() const=0;virtual ::jxx::lang::jbool isBootClassPathSupported() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getBootClassPath() const=0;virtual ::jxx::Ptr<::jxx::util::List<::jxx::lang::String>> getInputArguments() const=0;virtual ::jxx::lang::jlong getUptime() const=0;virtual ::jxx::lang::jlong getStartTime() const=0;virtual ::jxx::Ptr<::jxx::util::Map<::jxx::lang::String,::jxx::lang::String>> getSystemProperties() const=0;};
}
