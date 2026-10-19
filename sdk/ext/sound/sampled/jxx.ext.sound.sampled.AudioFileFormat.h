#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::ext::sound::sampled
{
    class AudioFileFormat final
        : public ::jxx::lang::ClassBase<AudioFileFormat, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AudioFileFormat, JxxSuper>;

        AudioFileFormat(
            const ::jxx::Ptr<::jxx::lang::String>& typeName,
            ::jxx::lang::jint byteLength);

        auto getByteLength() const -> ::jxx::lang::jint;
        auto getTypeName() const -> ::jxx::Ptr<::jxx::lang::String>;

    private:
        ::jxx::Ptr<::jxx::lang::String> typeName_;
        ::jxx::lang::jint byteLength_;
    };
}
