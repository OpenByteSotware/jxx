#pragma once
#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.List.h"
namespace jxx::lang::management { class PlatformLoggingMXBean : public ::jxx::lang::InterfaceBase<PlatformLoggingMXBean,PlatformManagedObject>{public:~PlatformLoggingMXBean() override=default;virtual ::jxx::Ptr<::jxx::util::List<::jxx::lang::String>> getLoggerNames() const=0;virtual ::jxx::Ptr<::jxx::lang::String> getLoggerLevel(const ::jxx::Ptr<::jxx::lang::String>&) const=0;virtual void setLoggerLevel(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<::jxx::lang::String>&)=0;virtual ::jxx::Ptr<::jxx::lang::String> getParentLoggerName(const ::jxx::Ptr<::jxx::lang::String>&) const=0;};}
