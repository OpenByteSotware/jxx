#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioSystem.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.UnsupportedAudioFileException.h"
#include "io/jxx.io.File.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Class.h"
#include <array>
#include <cstdio>
#include <fstream>

namespace
{
void writeLittleEndian32(std::ofstream& stream, std::uint32_t value)
{
    const std::array<unsigned char, 4> bytes{
        static_cast<unsigned char>(value & 0xffU),
        static_cast<unsigned char>((value >> 8U) & 0xffU),
        static_cast<unsigned char>((value >> 16U) & 0xffU),
        static_cast<unsigned char>((value >> 24U) & 0xffU)};
    stream.write(reinterpret_cast<const char*>(bytes.data()), 4);
}
}

TEST(AudioSystemHeadless, ReadsWaveByteLength)
{
    ::jxx::initialize();
    const char* path = "jxx_audio_system_test.wav";
    {
        std::ofstream stream(path, std::ios::binary);
        stream.write("RIFF", 4);
        writeLittleEndian32(stream, 40U);
        stream.write("WAVE", 4);
        stream.write("fmt ", 4);
        writeLittleEndian32(stream, 16U);
        const std::array<unsigned char, 16> format{};
        stream.write(reinterpret_cast<const char*>(format.data()), 16);
        stream.write("data", 4);
        writeLittleEndian32(stream, 4U);
        const std::array<unsigned char, 4> data{};
        stream.write(reinterpret_cast<const char*>(data.data()), 4);
    }
    auto file = ::jxx::NEW<::jxx::io::File>(::jxx::NEW<::jxx::lang::String>(path));
    auto format = ::jxx::ext::sound::sampled::AudioSystem::getAudioFileFormat(file);
    ASSERT_NE(format, nullptr);
    EXPECT_EQ(format->getByteLength(), 48);
    ASSERT_NE(format->getType(), nullptr);
    EXPECT_EQ(format->getType()->toString()->utf8(), "WAVE");
    EXPECT_EQ(format->getType()->getExtension()->utf8(), "wav");
    std::remove(path);
}

TEST(AudioSystemHeadless, RejectsNonWaveFile)
{
    const char* path = "jxx_audio_system_invalid.bin";
    { std::ofstream stream(path, std::ios::binary); stream.write("not-wave", 8); }
    auto file = ::jxx::NEW<::jxx::io::File>(::jxx::NEW<::jxx::lang::String>(path));
    EXPECT_THROW(
        ::jxx::ext::sound::sampled::AudioSystem::getAudioFileFormat(file),
        ::jxx::ext::sound::sampled::UnsupportedAudioFileException);
    std::remove(path);
}
