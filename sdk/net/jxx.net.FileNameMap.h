#pragma once

#include "jxx_types.h"
#include "jxx.lang.String.h"

namespace jxx::net
{
    class FileNameMap
    {
    public:
        virtual ~FileNameMap() = default;

    public:
        virtual jxx::Ptr<jxx::lang::String> getContentTypeFor(jxx::Ptr<jxx::lang::String> fileName) = 0;
    };
}
