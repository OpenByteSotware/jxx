#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioSystem.h"

#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.UnsupportedAudioFileException.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <limits>
#include <string>

namespace jxx::ext::sound::sampled
{
    namespace
    {
        auto readLittleEndian32(const std::array<unsigned char, 4>& bytes)
            -> std::uint32_t
        {
            return static_cast<std::uint32_t>(bytes[0]) |
                (static_cast<std::uint32_t>(bytes[1]) << 8U) |
                (static_cast<std::uint32_t>(bytes[2]) << 16U) |
                (static_cast<std::uint32_t>(bytes[3]) << 24U);
        }

        auto readFour(std::ifstream& stream) -> std::array<unsigned char, 4>
        {
            std::array<unsigned char, 4> bytes{};
            stream.read(reinterpret_cast<char*>(bytes.data()), 4);
            return bytes;
        }
    }

    auto AudioSystem::getAudioFileFormat(
        const ::jxx::Ptr<::jxx::io::File>& file)
        -> ::jxx::Ptr<AudioFileFormat>
    {
        if (file == nullptr)
            throw ::jxx::lang::NullPointerException("file");

        const auto path = file->getPath();
        std::ifstream stream(path->utf8(), std::ios::binary);
        if (!stream)
            throw ::jxx::io::IOException("Unable to open audio file");

        const auto riff = readFour(stream);
        const auto riffSizeBytes = readFour(stream);
        const auto wave = readFour(stream);
        if (!stream || std::string(reinterpret_cast<const char*>(riff.data()), 4) != "RIFF" ||
            std::string(reinterpret_cast<const char*>(wave.data()), 4) != "WAVE")
            throw UnsupportedAudioFileException("Unsupported audio file");

        const auto riffSize = readLittleEndian32(riffSizeBytes);
        const std::uint64_t totalLength = static_cast<std::uint64_t>(riffSize) + 8U;
        if (totalLength > static_cast<std::uint64_t>(std::numeric_limits<::jxx::lang::jint>::max()))
            throw UnsupportedAudioFileException("Audio file is too large");

        return ::jxx::NEW<AudioFileFormat>(
            ::jxx::NEW<::jxx::lang::String>("WAVE"),
            static_cast<::jxx::lang::jint>(totalLength));
    }
}
