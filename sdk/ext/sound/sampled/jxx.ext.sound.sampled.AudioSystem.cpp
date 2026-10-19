#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioSystem.h"

#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFormat.h"
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
        auto read16(std::ifstream& stream) -> std::uint16_t
        {
            std::array<unsigned char, 2> value{};
            stream.read(reinterpret_cast<char*>(value.data()), 2);
            return static_cast<std::uint16_t>(value[0]) |
                static_cast<std::uint16_t>(value[1] << 8U);
        }

        auto read32(std::ifstream& stream) -> std::uint32_t
        {
            std::array<unsigned char, 4> value{};
            stream.read(reinterpret_cast<char*>(value.data()), 4);
            return static_cast<std::uint32_t>(value[0]) |
                (static_cast<std::uint32_t>(value[1]) << 8U) |
                (static_cast<std::uint32_t>(value[2]) << 16U) |
                (static_cast<std::uint32_t>(value[3]) << 24U);
        }

        auto readTag(std::ifstream& stream) -> std::string
        {
            std::array<char, 4> value{};
            stream.read(value.data(), 4);
            return std::string(value.data(), value.size());
        }
    }

    auto AudioSystem::getAudioFileFormat(
        const ::jxx::Ptr<::jxx::io::File>& file)
        -> ::jxx::Ptr<AudioFileFormat>
    {
        if (file == nullptr)
            throw ::jxx::lang::NullPointerException("file");

        std::ifstream stream(file->getPath()->utf8(), std::ios::binary);
        if (!stream)
            throw ::jxx::io::IOException("Unable to open audio file");

        if (readTag(stream) != "RIFF")
            throw UnsupportedAudioFileException("Unsupported audio file");
        const auto riffSize = read32(stream);
        if (readTag(stream) != "WAVE")
            throw UnsupportedAudioFileException("Unsupported audio file");

        std::uint16_t audioEncoding = 0;
        std::uint16_t channels = 0;
        std::uint32_t sampleRate = 0;
        std::uint16_t blockAlign = 0;
        std::uint16_t sampleSize = 0;
        std::uint32_t dataLength = 0;
        bool foundFormat = false;
        bool foundData = false;

        while (stream && (!foundFormat || !foundData))
        {
            const auto tag = readTag(stream);
            if (!stream) break;
            const auto chunkLength = read32(stream);
            if (!stream) break;

            if (tag == "fmt " && chunkLength >= 16U)
            {
                audioEncoding = read16(stream);
                channels = read16(stream);
                sampleRate = read32(stream);
                (void)read32(stream);
                blockAlign = read16(stream);
                sampleSize = read16(stream);
                if (chunkLength > 16U)
                    stream.seekg(static_cast<std::streamoff>(chunkLength - 16U), std::ios::cur);
                foundFormat = true;
            }
            else if (tag == "data")
            {
                dataLength = chunkLength;
                stream.seekg(static_cast<std::streamoff>(chunkLength), std::ios::cur);
                foundData = true;
            }
            else
            {
                stream.seekg(static_cast<std::streamoff>(chunkLength), std::ios::cur);
            }

            if ((chunkLength & 1U) != 0U)
                stream.seekg(1, std::ios::cur);
        }

        if (!foundFormat || !foundData || channels == 0 || blockAlign == 0)
            throw UnsupportedAudioFileException("Incomplete WAVE file");

        ::jxx::Ptr<AudioFormat::Encoding> encoding;
        if (audioEncoding == 1U)
            encoding = sampleSize == 8U
                ? AudioFormat::Encoding::PCM_UNSIGNED
                : AudioFormat::Encoding::PCM_SIGNED;
        else if (audioEncoding == 3U)
            encoding = AudioFormat::Encoding::PCM_FLOAT;
        else if (audioEncoding == 6U)
            encoding = AudioFormat::Encoding::ALAW;
        else if (audioEncoding == 7U)
            encoding = AudioFormat::Encoding::ULAW;
        else
            throw UnsupportedAudioFileException("Unsupported WAVE encoding");

        const std::uint64_t byteLength = static_cast<std::uint64_t>(riffSize) + 8U;
        if (byteLength > static_cast<std::uint64_t>(std::numeric_limits<::jxx::lang::jint>::max()))
            throw UnsupportedAudioFileException("Audio file is too large");

        const auto format = ::jxx::NEW<AudioFormat>(
            encoding,
            static_cast<::jxx::lang::jfloat>(sampleRate),
            static_cast<::jxx::lang::jint>(sampleSize),
            static_cast<::jxx::lang::jint>(channels),
            static_cast<::jxx::lang::jint>(blockAlign),
            static_cast<::jxx::lang::jfloat>(sampleRate),
            false);

        return ::jxx::NEW<AudioFileFormat>(
            AudioFileFormat::Type::WAVE,
            static_cast<::jxx::lang::jint>(byteLength),
            format,
            static_cast<::jxx::lang::jint>(dataLength / blockAlign));
    }
}
