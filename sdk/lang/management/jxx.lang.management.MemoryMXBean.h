#pragma once
#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/management/jxx.lang.management.MemoryUsage.h"
namespace jxx::lang::management { class MemoryMXBean : public ::jxx::lang::InterfaceBase<MemoryMXBean,PlatformManagedObject>{public:~MemoryMXBean() override=default;virtual ::jxx::lang::jint getObjectPendingFinalizationCount() const=0;virtual ::jxx::Ptr<MemoryUsage> getHeapMemoryUsage() const=0;virtual ::jxx::Ptr<MemoryUsage> getNonHeapMemoryUsage() const=0;virtual ::jxx::lang::jbool isVerbose() const=0;virtual void setVerbose(::jxx::lang::jbool)=0;virtual void gc()=0;};}
