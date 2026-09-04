#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class ContentHandler;

    class ContentHandlerFactory : public jxx::lang::InterfaceBase<ContentHandlerFactory> {
    public:
        virtual ~ContentHandlerFactory() = default;

    public:
        virtual jxx::Ptr<ContentHandler> createContentHandler(const jxx::Ptr<jxx::lang::String>& mimeType) = 0;
    };
}
