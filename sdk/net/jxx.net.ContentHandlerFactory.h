#pragma once

#include "jxx_types.h"
#include "jxx.lang.String.h"

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
