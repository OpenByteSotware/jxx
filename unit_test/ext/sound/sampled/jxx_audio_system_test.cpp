#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioSystem.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.UnsupportedAudioFileException.h"
#include "io/jxx.io.File.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

#include <array>
#include <cstdio>
#include <cstdint>
#include <fstream>

namespace
{
class FileCleanup final
{
public:
    explicit FileCleanup(const char* path) : path_(path) {}
    ~FileCleanup() { std::remove(path_); }
private:
    const char* path_;
};

void writeLittleEndian16(std::ofstream& stream, std::uint16_t value)
{
    const std::array<unsigned char, 2> bytes{
        static_cast<unsigned char>(value & 0xffU),
        static_cast<unsigned char>((value >> 8U) & 0xffU)};
    stream.write(reinterpret_cast<const char*>(bytes.data()), 2);
}

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
    FileCleanup cleanup(path);
    {
        std::ofstream stream(path, std::ios::binary);
        ASSERT_TRUE(stream.is_open());
        stream.write("RIFF", 4);
        writeLittleEndian32(stream, 40U);
        stream.write("WAVE", 4);
        stream.write("fmt ", 4);
        writeLittleEndian32(stream, 16U);
        writeLittleEndian16(stream, 1U);      // PCM
        writeLittleEndian16(stream, 1U);      // mono
        writeLittleEndian32(stream, 8000U);   // sample rate
        writeLittleEndian32(stream, 8000U);   // byte rate
        writeLittleEndian16(stream, 1U);      // block alignment
        writeLittleEndian16(stream, 8U);      // bits per sample
        stream.write("data", 4);
        writeLittleEndian32(stream, 4U);
        const std::array<unsigned char, 4> data{};
        stream.write(reinterpret_cast<const char*>(data.data()), 4);
        ASSERT_TRUE(stream.good());
    }

    auto file = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<::jxx::lang::String>(path));
    auto format =
        ::jxx::ext::sound::sampled::AudioSystem::getAudioFileFormat(file);
    ASSERT_NE(format, nullptr);
    EXPECT_EQ(format->getByteLength(), 48);
    EXPECT_EQ(format->getFrameLength(), 4);
    ASSERT_NE(format->getType(), nullptr);
    EXPECT_EQ(format->getType()->toString()->utf8(), "WAVE");
    EXPECT_EQ(format->getType()->getExtension()->utf8(), "wav");
    ASSERT_NE(format->getFormat(), nullptr);
    EXPECT_EQ(format->getFormat()->getChannels(), 1);
    EXPECT_EQ(format->getFormat()->getFrameSize(), 1);
    EXPECT_EQ(format->getFormat()->getSampleSizeInBits(), 8);
}

TEST(AudioSystemHeadless, RejectsNonWaveFile)
{
    ::jxx::initialize();
    const char* path = "jxx_audio_system_invalid.bin";
    FileCleanup cleanup(path);
    {
        std::ofstream stream(path, std::ios::binary);
        ASSERT_TRUE(stream.is_open());
        stream.write("not-wave", 8);
        ASSERT_TRUE(stream.good());
    }
    auto file = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<::jxx::lang::String>(path));
    EXPECT_THROW(
        ::jxx::ext::sound::sampled::AudioSystem::getAudioFileFormat(file),
        ::jxx::ext::sound::sampled::UnsupportedAudioFileException);
}
