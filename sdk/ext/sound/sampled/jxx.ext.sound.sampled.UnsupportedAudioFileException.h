#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::ext::sound::sampled
{
    class UnsupportedAudioFileException final
        : public ::jxx::lang::ClassBase<
              UnsupportedAudioFileException,
              ::jxx::lang::Exception>
    {
    public:
        using JxxSuper = ::jxx::lang::Exception;
        using Super = ::jxx::lang::ClassBase<
            UnsupportedAudioFileException, JxxSuper>;

        UnsupportedAudioFileException();
        explicit UnsupportedAudioFileException(const char* message);
        explicit UnsupportedAudioFileException(
            const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
