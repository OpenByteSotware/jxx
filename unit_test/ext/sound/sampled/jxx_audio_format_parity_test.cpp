#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioSystem.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.UnsupportedAudioFileException.h"
#include "io/jxx.io.File.h"
#include "lang/jxx.lang.String.h"

#include <array>
#include <cstdio>
#include <cstdint>
#include <fstream>

namespace
{
void write16(std::ofstream& stream, std::uint16_t value)
{
    const std::array<unsigned char, 2> bytes{
        static_cast<unsigned char>(value & 0xffU),
        static_cast<unsigned char>((value >> 8U) & 0xffU)};
    stream.write(reinterpret_cast<const char*>(bytes.data()), 2);
}

void write32(std::ofstream& stream, std::uint32_t value)
{
    const std::array<unsigned char, 4> bytes{
        static_cast<unsigned char>(value & 0xffU),
        static_cast<unsigned char>((value >> 8U) & 0xffU),
        static_cast<unsigned char>((value >> 16U) & 0xffU),
        static_cast<unsigned char>((value >> 24U) & 0xffU)};
    stream.write(reinterpret_cast<const char*>(bytes.data()), 4);
}

void createWave(const char* path)
{
    std::ofstream stream(path, std::ios::binary);
    stream.write("RIFF", 4); write32(stream, 44U);
    stream.write("WAVE", 4);
    stream.write("fmt ", 4); write32(stream, 16U);
    write16(stream, 1U); write16(stream, 2U); write32(stream, 44100U);
    write32(stream, 176400U); write16(stream, 4U); write16(stream, 16U);
    stream.write("data", 4); write32(stream, 8U);
    const std::array<unsigned char, 8> samples{};
    stream.write(reinterpret_cast<const char*>(samples.data()), 8);
}
}

TEST(AudioFormatParity, WaveMetadataIsExposed)
{
    ::jxx::initialize();
    const char* path = "jxx_audio_format_parity.wav";
    createWave(path);

    auto file = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<::jxx::lang::String>(path));
    auto fileFormat =
        ::jxx::ext::sound::sampled::AudioSystem::getAudioFileFormat(file);

    ASSERT_NE(fileFormat, nullptr);
    EXPECT_EQ(fileFormat->getType(),
        ::jxx::ext::sound::sampled::AudioFileFormat::Type::WAVE);
    EXPECT_EQ(fileFormat->getType()->getExtension()->utf8(), "wav");
    EXPECT_EQ(fileFormat->getByteLength(), 52);
    EXPECT_EQ(fileFormat->getFrameLength(), 2);

    auto format = fileFormat->getFormat();
    ASSERT_NE(format, nullptr);
    EXPECT_EQ(format->getEncoding(),
        ::jxx::ext::sound::sampled::AudioFormat::Encoding::PCM_SIGNED);
    EXPECT_FLOAT_EQ(format->getSampleRate(), 44100.0F);
    EXPECT_EQ(format->getSampleSizeInBits(), 16);
    EXPECT_EQ(format->getChannels(), 2);
    EXPECT_EQ(format->getFrameSize(), 4);
    EXPECT_FLOAT_EQ(format->getFrameRate(), 44100.0F);
    EXPECT_FALSE(format->isBigEndian());

    std::remove(path);
}

TEST(AudioFormatParity, EightBitPcmIsUnsigned)
{
    auto format = ::jxx::NEW<::jxx::ext::sound::sampled::AudioFormat>(
        ::jxx::ext::sound::sampled::AudioFormat::Encoding::PCM_UNSIGNED,
        8000.0F, 8, 1, 1, 8000.0F, false);
    EXPECT_EQ(format->getEncoding()->toString()->utf8(), "PCM_UNSIGNED");
}
