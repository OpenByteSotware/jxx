#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::test::process {

inline jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1>>
commandArray(const std::vector<std::string>& values) {
    auto result = jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1>>(
        static_cast<std::uint32_t>(values.size()));
    for (std::uint32_t index = 0; index < result->length; ++index) {
        (*result)[index] = jxx::NEW<jxx::lang::String>(values[index]);
    }
    return result;
}

inline jxx::Ptr<jxx::lang::ProcessBuilder>
builder(const std::vector<std::string>& values) {
    return jxx::NEW<jxx::lang::ProcessBuilder>(commandArray(values));
}

inline std::vector<std::string> shellCommand(const std::string& script) {
#ifdef _WIN32
    return {"cmd.exe", "/d", "/s", "/c", script};
#else
    return {"/bin/sh", "-c", script};
#endif
}

inline std::vector<std::string> sleepCommand() {
#ifdef _WIN32
    return shellCommand("ping -n 6 127.0.0.1 > nul");
#else
    return shellCommand("sleep 5");
#endif
}

inline std::string readAll(const jxx::Ptr<jxx::io::InputStream>& input) {
    std::string result;
    for (;;) {
        const auto value = input->read();
        if (value < 0) break;
        result.push_back(static_cast<char>(value));
    }
    return result;
}

inline std::filesystem::path temporaryPath(const std::string& suffix) {
    static unsigned long counter = 0;
    return std::filesystem::temp_directory_path() /
        ("jxx_process_test_" + std::to_string(++counter) + suffix);
}

} // namespace jxx::test::process
