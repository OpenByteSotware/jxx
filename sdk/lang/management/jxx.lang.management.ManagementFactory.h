#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.List.h"
#include "lang/management/jxx.lang.management.ClassLoadingMXBean.h"
#include "lang/management/jxx.lang.management.CompilationMXBean.h"
#include "lang/management/jxx.lang.management.GarbageCollectorMXBean.h"
#include "lang/management/jxx.lang.management.MemoryManagerMXBean.h"
#include "lang/management/jxx.lang.management.MemoryMXBean.h"
#include "lang/management/jxx.lang.management.MemoryPoolMXBean.h"
#include "lang/management/jxx.lang.management.OperatingSystemMXBean.h"
#include "lang/management/jxx.lang.management.RuntimeMXBean.h"
#include "lang/management/jxx.lang.management.ThreadMXBean.h"
namespace jxx::lang::management {
class ManagementFactory final:public ::jxx::lang::ClassBase<ManagementFactory,::jxx::lang::Object>{public:
 static ::jxx::Ptr<::jxx::lang::String> CLASS_LOADING_MXBEAN_NAME,COMPILATION_MXBEAN_NAME,MEMORY_MXBEAN_NAME,OPERATING_SYSTEM_MXBEAN_NAME,RUNTIME_MXBEAN_NAME,THREAD_MXBEAN_NAME,GARBAGE_COLLECTOR_MXBEAN_DOMAIN_TYPE,MEMORY_MANAGER_MXBEAN_DOMAIN_TYPE,MEMORY_POOL_MXBEAN_DOMAIN_TYPE;
 static ::jxx::Ptr<ClassLoadingMXBean> getClassLoadingMXBean();static ::jxx::Ptr<MemoryMXBean> getMemoryMXBean();static ::jxx::Ptr<ThreadMXBean> getThreadMXBean();static ::jxx::Ptr<RuntimeMXBean> getRuntimeMXBean();static ::jxx::Ptr<CompilationMXBean> getCompilationMXBean();static ::jxx::Ptr<OperatingSystemMXBean> getOperatingSystemMXBean();static ::jxx::Ptr<::jxx::util::List<MemoryPoolMXBean>> getMemoryPoolMXBeans();static ::jxx::Ptr<::jxx::util::List<MemoryManagerMXBean>> getMemoryManagerMXBeans();static ::jxx::Ptr<::jxx::util::List<GarbageCollectorMXBean>> getGarbageCollectorMXBeans();
private:ManagementFactory()=delete;};}
