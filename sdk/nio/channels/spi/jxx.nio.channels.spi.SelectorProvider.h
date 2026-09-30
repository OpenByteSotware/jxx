#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::nio::channels {
class Selector;
class ServerSocketChannel;
class SocketChannel;
}

namespace jxx::nio::channels::spi {

class SelectorProvider
    : public ::jxx::lang::ClassBase<
          SelectorProvider,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;

    ~SelectorProvider() override = default;

    static ::jxx::Ptr<SelectorProvider> provider();

    virtual ::jxx::Ptr<::jxx::nio::channels::SocketChannel>
    openSocketChannel() = 0;
    virtual ::jxx::Ptr<::jxx::nio::channels::ServerSocketChannel>
    openServerSocketChannel() = 0;
    virtual ::jxx::Ptr<::jxx::nio::channels::Selector>
    openSelector() = 0;

protected:
    SelectorProvider() = default;
};

} // namespace jxx::nio::channels::spi
