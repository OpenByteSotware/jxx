#include "ext/sound/sampled/jxx.ext.sound.sampled.UnsupportedAudioFileException.h"

namespace jxx::ext::sound::sampled
{
    UnsupportedAudioFileException::UnsupportedAudioFileException()
        : Super()
    {
    }

    UnsupportedAudioFileException::UnsupportedAudioFileException(
        const char* message)
        : Super(message)
    {
    }

    UnsupportedAudioFileException::UnsupportedAudioFileException(
        const ::jxx::Ptr<::jxx::lang::String>& message)
        : Super(message)
    {
    }
}
