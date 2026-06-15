#include "net/jxx.net.IDN.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace
{
    inline std::string asciiFold_(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        return s;
    }
}

namespace jxx::net
{
    jxx::Ptr<jxx::lang::String> IDN::toASCII(jxx::Ptr<jxx::lang::String> input)
    {
        return toASCII(std::move(input), 0);
    }

    jxx::Ptr<jxx::lang::String> IDN::toASCII(jxx::Ptr<jxx::lang::String> input,
                                             jxx::lang::jint /*flags*/)
    {
        if (!input)
            throw std::invalid_argument("null input");
        return std::make_shared<jxx::lang::String>(asciiFold_(input->utf8()));
    }

    jxx::Ptr<jxx::lang::String> IDN::toUnicode(jxx::Ptr<jxx::lang::String> input)
    {
        return toUnicode(std::move(input), 0);
    }

    jxx::Ptr<jxx::lang::String> IDN::toUnicode(jxx::Ptr<jxx::lang::String> input,
                                               jxx::lang::jint /*flags*/)
    {
        if (!input)
            throw std::invalid_argument("null input");
        return input;
    }
}
