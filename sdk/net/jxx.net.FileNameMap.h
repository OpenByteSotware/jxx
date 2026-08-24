#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class FileNameMap
    {
    public:
        virtual ~FileNameMap() = default;

    public:
        virtual jxx::Ptr<jxx::lang::String> getContentTypeFor(const jxx::Ptr<jxx::lang::String> fileName) = 0;
    };
}
