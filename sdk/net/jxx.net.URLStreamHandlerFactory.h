#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class URLStreamHandler;

    class URLStreamHandlerFactory : public jxx::lang::InterfaceBase<URLStreamHandlerFactory> {
    public:
        virtual ~URLStreamHandlerFactory() = default;

    public:
        virtual jxx::Ptr<URLStreamHandler> createURLStreamHandler(const jxx::Ptr<jxx::lang::String>& protocol) = 0;
    };
}
