#pragma once
#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.String.h"
namespace jxx::lang::management { class MemoryManagerMXBean : public ::jxx::lang::InterfaceBase<MemoryManagerMXBean,PlatformManagedObject>{public:~MemoryManagerMXBean() override=default;virtual ::jxx::Ptr<::jxx::lang::String> getName() const=0;virtual ::jxx::lang::jbool isValid() const=0;virtual ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>,1U>> getMemoryPoolNames() const=0;};}
