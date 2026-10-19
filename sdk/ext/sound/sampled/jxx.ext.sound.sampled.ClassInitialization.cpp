#include "ext/sound/sampled/jxx.ext.sound.sampled.ClassInitialization.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.UnsupportedAudioFileException.h"
#include <mutex>
namespace jxx::ext::sound::sampled
{
    void initializeClasses()
    {
        static std::once_flag flag;
        std::call_once(flag, []()
        {
            (void)AudioFormat::Class();
            (void)AudioFormat::Encoding::Class();
            (void)AudioFileFormat::Class();
            (void)AudioFileFormat::Type::Class();
            (void)UnsupportedAudioFileException::Class();
        });
    }
}
