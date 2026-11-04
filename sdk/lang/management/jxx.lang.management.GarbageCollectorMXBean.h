#pragma once
#include "lang/management/jxx.lang.management.MemoryManagerMXBean.h"
namespace jxx::lang::management { class GarbageCollectorMXBean : public ::jxx::lang::InterfaceBase<GarbageCollectorMXBean,MemoryManagerMXBean>{public:~GarbageCollectorMXBean() override=default;virtual ::jxx::lang::jlong getCollectionCount() const=0;virtual ::jxx::lang::jlong getCollectionTime() const=0;};}
