#include "ext/sound/sampled/jxx.ext.sound.sampled.ClassInitialization.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.UnsupportedAudioFileException.h"
#include <mutex>
namespace jxx::ext::sound::sampled { void initializeClasses(){ static std::once_flag flag; std::call_once(flag,[]{ (void)AudioFileFormat::Class(); (void)UnsupportedAudioFileException::Class(); }); } }
