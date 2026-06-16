#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class ContentHandler;

    class ContentHandlerFactory
    {
    public:
        virtual ~ContentHandlerFactory() = default;

    public:
        virtual jxx::Ptr<ContentHandler> createContentHandler(jxx::Ptr<jxx::lang::String> mimeType) = 0;
    };
}
