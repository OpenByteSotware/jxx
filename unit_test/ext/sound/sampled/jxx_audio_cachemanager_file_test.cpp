#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>

#include "jxx.initialize.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"
#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioSystem.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileInputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.CRC32.h"

namespace {
void write16(std::ofstream& output, std::uint16_t value) {
    output.put(static_cast<char>(value & 0xffU));
    output.put(static_cast<char>((value >> 8U) & 0xffU));
}
void write32(std::ofstream& output, std::uint32_t value) {
    for (unsigned shift = 0; shift < 32; shift += 8)
        output.put(static_cast<char>((value >> shift) & 0xffU));
}
}

TEST(AudioCacheManagerParity, FileLengthReadAndCrcWorkflow)
{
    ::jxx::initialize();
    const auto path = std::filesystem::temp_directory_path() /
        "jxx_cachemanager_audio_test.wav";
    {
        std::ofstream output(path, std::ios::binary | std::ios::trunc);
        output.write("RIFF", 4); write32(output, 36U + 4U);
        output.write("WAVEfmt ", 8); write32(output, 16);
        write16(output, 1); write16(output, 1); write32(output, 8000);
        write32(output, 8000); write16(output, 1); write16(output, 8);
        output.write("data", 4); write32(output, 4);
        const std::array<char, 4> payload{{0, 1, 2, 3}};
        output.write(payload.data(), payload.size());
    }

    auto file = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<::jxx::lang::String>(path.u8string()));
    auto format = ::jxx::ext::sound::sampled::AudioSystem::getAudioFileFormat(file);
    ASSERT_NE(format, nullptr);
    EXPECT_EQ(format->getByteLength(), 48);

    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(format->getByteLength());
    auto input = ::jxx::NEW<::jxx::io::FileInputStream>(file);
    ::jxx::lang::jint count = 0;
    while (count < bytes->length) {
        const auto read = input->read(bytes, count, bytes->length - count);
        if (read < 0) break;
        count += read;
    }
    input->close();
    EXPECT_EQ(count, format->getByteLength());
    auto crc = ::jxx::NEW<::jxx::util::zip::CRC32>();
    crc->update(bytes);
    EXPECT_NE(crc->getValue(), 0);
    std::filesystem::remove(path);
}
