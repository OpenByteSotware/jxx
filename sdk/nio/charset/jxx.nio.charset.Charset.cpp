#include <cctype>
#include <stdexcept>
#include "lang/jxx.lang.String.h"
#include "nio/charset/jxx.nio.charset.CharsetDecoder.h"
#include "nio/charset/jxx.nio.charset.CharsetEncoder.h"
#include "nio/charset/jxx.nio.charset.Charset.h"

namespace
{
    std::string up_(std::string s)
    {
        for (char& ch : s)
            ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        return s;
    }

    void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }
}

namespace jxx::nio::charset
{
    Charset::Charset(jxx::Ptr<jxx::lang::String> canonicalName,
        const jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> aliases)
        : canonicalName_(std::move(canonicalName)), aliases_(aliases)
    {
    }

    jxx::lang::jbool Charset::isSupported(jxx::Ptr<jxx::lang::String> charsetName)
    {
        try
        {
            return static_cast<jxx::lang::jbool>(forName(std::move(charsetName)) != nullptr);
        }
        catch (...)
        {
            return false;
        }
    }

    jxx::Ptr<Charset> Charset::forName(jxx::Ptr<jxx::lang::String> charsetName)
    {
        if (!charsetName)
            throwIAE_("null charset name");
        const auto n = up_(charsetName->utf8());
        if (n == "UTF-8" || n == "UTF8")
            return jxx::NEW<Charset>(jxx::NEW<jxx::lang::String>("UTF-8"), jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(0));
        if (n == "UTF-16" || n == "UTF16")
            return jxx::NEW<Charset>(jxx::NEW<jxx::lang::String>("UTF-16"), jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(0));
        if (n == "UTF-16BE" || n == "UTF16BE")
            return jxx::NEW<Charset>(jxx::NEW<jxx::lang::String>("UTF-16BE"), jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(0) );
        if (n == "UTF-16LE" || n == "UTF16LE")
            return jxx::NEW<Charset>(jxx::NEW<jxx::lang::String>("UTF-16LE"), jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(0));
        if (n == "US-ASCII" || n == "ASCII" || n == "US_ASCII")
            return jxx::NEW<Charset>(jxx::NEW<jxx::lang::String>("US-ASCII"), jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(0));
        if (n == "ISO-8859-1" || n == "ISO8859-1" || n == "LATIN1")
            return jxx::NEW<Charset>(jxx::NEW<jxx::lang::String>("ISO-8859-1"), jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(0));
        throwIAE_("unsupported charset");
    }

    jxx::Ptr<Charset> Charset::defaultCharset()
    {
        return forName(jxx::NEW<jxx::lang::String>("UTF-8"));
    }

    jxx::Ptr<jxx::lang::String> Charset::name() const { return canonicalName_; }
    jxx::lang::jbool Charset::contains(jxx::Ptr<Charset> cs) const
    {
        return cs && canonicalName_ && cs->canonicalName_ && canonicalName_->equals(cs->canonicalName_);
    }

    jxx::Ptr<CharsetDecoder> Charset::newDecoder()
    {
        return jxx::NEW<CharsetDecoder>(jxx::CAST<Charset, jxx::lang::Object>(this->thisPtr));
    }

    jxx::Ptr<CharsetEncoder> Charset::newEncoder()
    {
        return jxx::NEW<CharsetEncoder>(jxx::CAST<Charset, jxx::lang::Object>(this->thisPtr));
    }

    jxx::Ptr<jxx::lang::String> Charset::toString() const { return canonicalName_; }
    jxx::lang::jbool Charset::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto o = std::dynamic_pointer_cast<Charset>(other);
        return o && canonicalName_ && o->canonicalName_ && canonicalName_->equals(o->canonicalName_);
    }
    jxx::lang::jint Charset::hashCode() const { return canonicalName_ ? canonicalName_->hashCode() : 0; }
}
