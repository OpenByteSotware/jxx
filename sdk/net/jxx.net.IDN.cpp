#include <algorithm>
#include <cctype>

#include "net/jxx.net.IDN.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

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
    jxx::Ptr<jxx::lang::String> IDN::toASCII(const jxx::Ptr<jxx::lang::String>& input)
    {
        return toASCII(std::move(input), 0);
    }

    jxx::Ptr<jxx::lang::String> IDN::toASCII(const jxx::Ptr<jxx::lang::String>& input,
                                             jxx::lang::jint flags)
    {
        if (input == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if ((flags & ~(ALLOW_UNASSIGNED | USE_STD3_ASCII_RULES)) != 0) {
            throw jxx::lang::IllegalArgumentException("unsupported IDN flags");
        }
        return jxx::NEW<jxx::lang::String>(asciiFold_(input->utf8()));
    }

    jxx::Ptr<jxx::lang::String> IDN::toUnicode(const jxx::Ptr<jxx::lang::String>& input)
    {
        return toUnicode(std::move(input), 0);
    }

    jxx::Ptr<jxx::lang::String> IDN::toUnicode(const jxx::Ptr<jxx::lang::String>& input,
                                               jxx::lang::jint flags)
    {
        if (input == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if ((flags & ~(ALLOW_UNASSIGNED | USE_STD3_ASCII_RULES)) != 0) {
            throw jxx::lang::IllegalArgumentException("unsupported IDN flags");
        }
        return input;
    }
}
