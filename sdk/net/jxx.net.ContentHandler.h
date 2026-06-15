#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::lang
{
    class Class;
}

namespace jxx::net
{
    class URLConnection;

    class ContentHandler : public jxx::lang::Object
    {
    public:
        ~ContentHandler() override = default;

    public:
        virtual jxx::Ptr<jxx::lang::Object> getContent(jxx::Ptr<URLConnection> urlc) = 0;
        virtual jxx::Ptr<jxx::lang::Object> getContent(
            jxx::Ptr<URLConnection> urlc,
            jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::Class>, 1U>> /*classes*/)
        {
            return getContent(std::move(urlc));
        }
    };
}
