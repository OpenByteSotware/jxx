#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"

namespace jxx::ext::sound::sampled
{
    AudioFileFormat::AudioFileFormat(
        const ::jxx::Ptr<::jxx::lang::String>& typeName,
        ::jxx::lang::jint byteLength)
        : Super(), typeName_(typeName), byteLength_(byteLength)
    {
    }

    auto AudioFileFormat::getByteLength() const -> ::jxx::lang::jint
    {
        return byteLength_;
    }

    auto AudioFileFormat::getTypeName() const
        -> ::jxx::Ptr<::jxx::lang::String>
    {
        return typeName_;
    }
}
