#include "lang/management/jxx.lang.management.ManagementFactory.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
namespace jxx::lang::management {
#define S(n,v) ::jxx::Ptr<::jxx::lang::String> ManagementFactory::n=::jxx::NEW<::jxx::lang::String>(v)
S(CLASS_LOADING_MXBEAN_NAME,"java.lang:type=ClassLoading");S(COMPILATION_MXBEAN_NAME,"java.lang:type=Compilation");S(MEMORY_MXBEAN_NAME,"java.lang:type=Memory");S(OPERATING_SYSTEM_MXBEAN_NAME,"java.lang:type=OperatingSystem");S(RUNTIME_MXBEAN_NAME,"java.lang:type=Runtime");S(THREAD_MXBEAN_NAME,"java.lang:type=Threading");S(GARBAGE_COLLECTOR_MXBEAN_DOMAIN_TYPE,"java.lang:type=GarbageCollector");S(MEMORY_MANAGER_MXBEAN_DOMAIN_TYPE,"java.lang:type=MemoryManager");S(MEMORY_POOL_MXBEAN_DOMAIN_TYPE,"java.lang:type=MemoryPool");
#undef S
[[noreturn]] static void unavailable(){throw ::jxx::lang::UnsupportedOperationException("platform MXBean backend is not implemented");}
#define G(t,n) ::jxx::Ptr<t> ManagementFactory::n(){unavailable();}
G(ClassLoadingMXBean,getClassLoadingMXBean) G(MemoryMXBean,getMemoryMXBean) G(ThreadMXBean,getThreadMXBean) G(RuntimeMXBean,getRuntimeMXBean) G(CompilationMXBean,getCompilationMXBean) G(OperatingSystemMXBean,getOperatingSystemMXBean)
#undef G
::jxx::Ptr<::jxx::util::List<MemoryPoolMXBean>> ManagementFactory::getMemoryPoolMXBeans(){unavailable();}
::jxx::Ptr<::jxx::util::List<MemoryManagerMXBean>> ManagementFactory::getMemoryManagerMXBeans(){unavailable();}
::jxx::Ptr<::jxx::util::List<GarbageCollectorMXBean>> ManagementFactory::getGarbageCollectorMXBeans(){unavailable();}
}
