#pragma once

#include <functional>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"

namespace jxx::com::sun::net::httpserver::internal {

class ServerConnectionTask final
    : public ::jxx::lang::ClassBase<
          ServerConnectionTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ServerConnectionTask,
        JxxSuper,
        ::jxx::lang::Runnable>;

    explicit ServerConnectionTask(std::function<void()> action);
    void run() override;

private:
    std::function<void()> action_;
};

} // namespace jxx::com::sun::net::httpserver::internal
