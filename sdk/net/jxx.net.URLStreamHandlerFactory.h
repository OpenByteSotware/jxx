#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class URLStreamHandler;

    class URLStreamHandlerFactory
    {
    public:
        virtual ~URLStreamHandlerFactory() = default;

    public:
        virtual jxx::Ptr<URLStreamHandler> createURLStreamHandler(jxx::Ptr<jxx::lang::String> protocol) = 0;
    };
}
