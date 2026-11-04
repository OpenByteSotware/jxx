#pragma once
#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/jxx.lang.String.h"
namespace jxx::lang::management { class BufferPoolMXBean : public ::jxx::lang::InterfaceBase<BufferPoolMXBean,PlatformManagedObject>{public:~BufferPoolMXBean() override=default;virtual ::jxx::Ptr<::jxx::lang::String> getName() const=0;virtual ::jxx::lang::jlong getCount() const=0;virtual ::jxx::lang::jlong getTotalCapacity() const=0;virtual ::jxx::lang::jlong getMemoryUsed() const=0;};}
