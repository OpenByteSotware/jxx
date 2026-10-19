#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx::io { class File; }
namespace jxx::ext::sound::sampled { class AudioFileFormat; }

namespace jxx::ext::sound::sampled
{
    class AudioSystem final
    {
    public:
        static auto getAudioFileFormat(
            const ::jxx::Ptr<::jxx::io::File>& file)
            -> ::jxx::Ptr<AudioFileFormat>;
    };
}
