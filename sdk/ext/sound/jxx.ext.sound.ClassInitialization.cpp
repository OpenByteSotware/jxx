#include "ext/sound/jxx.ext.sound.ClassInitialization.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.ClassInitialization.h"

namespace jxx::ext::sound
{
void initialize()
{
    ::jxx::ext::sound::sampled::initialize();
}

} // namespace jxx::ext::sound
