#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <cstddef>

#include <gtest/gtest.h>

#include "util/jxx.util.Properties.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.PrintWriter.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace {

using jxx::Ptr;
using jxx::lang::String;
using jxx::util::Properties;

static Ptr<String> S(const char* s) {
    return std::make_shared<String>(s);
}


namespace
{

    class MemoryReader final
        : public ::jxx::io::Reader
    {
    public:
        explicit MemoryReader(
            std::u16string text)
            : text_(std::move(text))
            , position_(0)
        {
        }

        ~MemoryReader() override = default;

        using ::jxx::io::Reader::read;

        ::jxx::lang::jint read() override
        {
            if (position_ >= text_.size()) {
                return -1;
            }

            return static_cast<::jxx::lang::jint>(
                text_[position_++]);
        }

        ::jxx::lang::jint read(
            const ::jxx::lang::CharArray& buffer,
            ::jxx::lang::jint offset,
            ::jxx::lang::jint length) override
        {

            validateBounds_(
                buffer,
                offset,
                length);

            if (length == 0) {
                return 0;
            }

            if (position_ >= text_.size()) {
                return -1;
            }

            const auto remaining =
                text_.size() - position_;

            const auto requested =
                static_cast<std::size_t>(length);

            const auto count =
                remaining < requested
                ? remaining
                : requested;

            for (std::size_t index = 0;
                 index < count;
                 ++index) {

                (*buffer)[
                    offset +
                        static_cast<::jxx::lang::jint>(index)] =
                    static_cast<::jxx::lang::jchar>(
                        text_[position_ + index]);
            }

            position_ += count;

            return static_cast<::jxx::lang::jint>(
                count);
        }

        void close() override
        {
        }

    private:
        static void validateBounds_(
            const ::jxx::lang::CharArray& buffer,
            ::jxx::lang::jint offset,
            ::jxx::lang::jint length)
        {

            if (buffer == nullptr) {
                throw ::jxx::lang::
                    NullPointerException();
            }

            const auto bufferLength =
                static_cast<::jxx::lang::jint>(
                    buffer->length);

            if (offset < 0 ||
                length < 0 ||
                offset > bufferLength - length) {

                throw ::jxx::lang::
                    IndexOutOfBoundsException();
            }
        }

        std::u16string text_;
        std::size_t position_;
    };

    class MemoryInputStream final
        : public ::jxx::io::InputStream
    {
    public:
        explicit MemoryInputStream(
            std::vector<unsigned char> bytes)
            : bytes_(std::move(bytes))
            , position_(0)
        {
        }

        ~MemoryInputStream() override = default;

        using ::jxx::io::InputStream::read;

        ::jxx::lang::jint read() override
        {
            if (position_ >= bytes_.size()) {
                return -1;
            }

            return static_cast<::jxx::lang::jint>(
                bytes_[position_++]);
        }

        void close() override
        {
        }

    private:
        std::vector<unsigned char> bytes_;
        std::size_t position_;
    };

    class MemoryWriter final
        : public ::jxx::io::Writer
    {
    public:
        MemoryWriter()
            : flushed_(false)
        {
        }

        ~MemoryWriter() override = default;

        using ::jxx::io::Writer::write;

        void write(
            ::jxx::lang::jint value) override
        {

            text_.push_back(
                static_cast<char16_t>(value));
        }

        void write(
            const ::jxx::lang::CharArray& buffer,
            ::jxx::lang::jint offset,
            ::jxx::lang::jint length) override
        {

            validateBounds_(
                buffer,
                offset,
                length);

            for (::jxx::lang::jint index = 0;
                 index < length;
                 ++index) {

                text_.push_back(
                    static_cast<char16_t>(
                        (*buffer)[offset + index]));
            }
        }

        void flush() override
        {
            flushed_ = true;
        }

        void close() override
        {
        }

        std::string utf8() const
        {
            return ::jxx::NEW<
                ::jxx::lang::String>(
                    text_)->utf8();
        }

        bool flushed() const
        {
            return flushed_;
        }

    private:
        static void validateBounds_(
            const ::jxx::lang::CharArray& buffer,
            ::jxx::lang::jint offset,
            ::jxx::lang::jint length)
        {

            if (buffer == nullptr) {
                throw ::jxx::lang::
                    NullPointerException();
            }

            const auto bufferLength =
                static_cast<::jxx::lang::jint>(
                    buffer->length);

            if (offset < 0 ||
                length < 0 ||
                offset > bufferLength - length) {

                throw ::jxx::lang::
                    IndexOutOfBoundsException();
            }
        }

        std::u16string text_;
        bool flushed_;
    };

    class MemoryOutputStream final
        : public ::jxx::io::OutputStream
    {
    public:
        MemoryOutputStream()
            : flushed_(false)
        {
        }

        ~MemoryOutputStream() override = default;

        using ::jxx::io::OutputStream::write;

        void write(
            ::jxx::lang::jint value) override
        {

            bytes_.push_back(
                static_cast<unsigned char>(
                    value & 0xFF));
        }

        void flush() override
        {
            flushed_ = true;
        }

        void close() override
        {
        }

        std::string bytesAsString() const
        {
            return std::string(
                bytes_.begin(),
                bytes_.end());
        }

        bool flushed() const
        {
            return flushed_;
        }

    private:
        std::vector<unsigned char> bytes_;
        bool flushed_;
    };

} // namespace

static std::vector<unsigned char> bytesFromAscii(const std::string& s) {
    return std::vector<unsigned char>(s.begin(), s.end());
}

static std::vector<unsigned char> bytesFromUtf8(const std::string& s) {
    return std::vector<unsigned char>(s.begin(), s.end());
}

class PropertiesTest : public ::testing::Test {
protected:
    void ExpectStringEq(const Ptr<String>& actual, const char* expected) {
        ASSERT_NE(actual, nullptr);
        EXPECT_EQ(actual->utf8(), std::string(expected));
    }
};

TEST_F(PropertiesTest, SetAndGetProperty) {
    auto p = jxx::NEW<Properties>();
    p->setProperty(S("alpha"), S("1"));
    ExpectStringEq(p->getProperty(S("alpha")), "1");
    ExpectStringEq(p->getProperty(S("missing"), S("fallback")), "fallback");
}

TEST_F(PropertiesTest, DefaultsChainLookup) {
    auto defaults = std::make_shared<Properties>();
    defaults->setProperty(S("host"), S("localhost"));
    auto p = std::make_shared<Properties>(defaults);
    ExpectStringEq(p->getProperty(S("host")), "localhost");
    p->setProperty(S("host"), S("override"));
    ExpectStringEq(p->getProperty(S("host")), "override");
}

TEST_F(PropertiesTest, PropertyNamesIncludesDefaultsWithoutDuplicates) {
    auto defaults = std::make_shared<Properties>();
    defaults->setProperty(S("a"), S("1"));
    defaults->setProperty(S("b"), S("2"));
    auto p = std::make_shared<Properties>(defaults);
    p->setProperty(S("b"), S("20"));
    p->setProperty(S("c"), S("3"));

    auto names = p->propertyNames();
    std::vector<std::string> got;
    while (names->hasMoreElements()) {
        auto obj = names->nextElement();
        auto s = jxx::CAST<String>(obj);
        if (s != nullptr) got.push_back(s->utf8());
    }
    auto has = [&](const std::string& key) { return std::find(got.begin(), got.end(), key) != got.end(); };
    EXPECT_TRUE(has("a"));
    EXPECT_TRUE(has("b"));
    EXPECT_TRUE(has("c"));
    EXPECT_EQ(got.size(), 3U);
}

TEST_F(PropertiesTest, StringPropertyNamesOnlyIncludesStringPairs) {
    auto p = std::make_shared<Properties>();
    p->setProperty(S("name"), S("jxx"));
    p->put(jxx::CAST<jxx::lang::Object>(S("string-key")), jxx::CAST<jxx::lang::Object>(std::make_shared<jxx::lang::Object>()));
    auto names = p->stringPropertyNames();
    auto it = names->iterator();
    std::vector<std::string> got;
    while (it->hasNext()) got.push_back(it->next()->utf8());
    ASSERT_EQ(got.size(), 1U);
    EXPECT_EQ(got[0], "name");
}

TEST_F(PropertiesTest, LoadReaderParsesEscapesContinuationsAndSeparators) {
    const std::u16string text =
        u"# comment\n"
        u"a=1\n"
        u"b:two\n"
        u"c three\n"
        u"multi=line1\\\n   line2\n"
        u"escaped\\ key=hello\\ world\n"
        u"unicode=\\u0041\\u0042\\u0043\n"
        u"tab=one\\ttwo\n";
    auto reader = std::make_shared<MemoryReader>(text);
    auto p = std::make_shared<Properties>();
    p->load(jxx::CAST<jxx::io::Reader>(reader));
    ExpectStringEq(p->getProperty(S("a")), "1");
    ExpectStringEq(p->getProperty(S("b")), "two");
    ExpectStringEq(p->getProperty(S("c")), "three");
    ExpectStringEq(p->getProperty(S("multi")), "line1line2");
    ExpectStringEq(p->getProperty(S("escaped key")), "hello world");
    ExpectStringEq(p->getProperty(S("unicode")), "ABC");
    ExpectStringEq(p->getProperty(S("tab")), "one\ttwo");
}

TEST_F(PropertiesTest, LoadInputStreamUsesLatin1StyleMapping) {
    std::string raw = std::string("latin=") + char(0xE9) + "\n";
    auto in = std::make_shared<MemoryInputStream>(bytesFromAscii(raw));
    auto p = std::make_shared<Properties>();
    p->load(jxx::CAST<jxx::io::InputStream>(in));
    auto value = p->getProperty(S("latin"));
    ASSERT_NE(value, nullptr);
    ASSERT_EQ(value->utf16().size(), 1U);
    EXPECT_EQ(value->utf16()[0], 0x00E9);
}

TEST_F(PropertiesTest, StoreWriterEscapesAndEmitsComments) {
    auto p = std::make_shared<Properties>();
    p->setProperty(S("space key"), S("value with spaces"));
    p->setProperty(S("unicode"), std::make_shared<String>(u"A\u03A9"));
    auto writer = std::make_shared<MemoryWriter>();
    p->store(jxx::CAST<jxx::io::Writer>(writer), std::make_shared<String>(u"line1\nline2"));
    const std::string out = writer->utf8();
    EXPECT_NE(out.find("#line1"), std::string::npos);
    EXPECT_NE(out.find("#line2"), std::string::npos);
    EXPECT_NE(out.find("space\\ key=value with spaces"), std::string::npos);
    EXPECT_NE(out.find("unicode=A"), std::string::npos);
    EXPECT_TRUE(writer->flushed());
}

TEST_F(PropertiesTest, StoreOutputStreamEmitsAsciiSafeForm) {
    auto p = std::make_shared<Properties>();
    p->setProperty(S("k"), std::make_shared<String>(u"\u03A9"));
    auto out = std::make_shared<MemoryOutputStream>();
    p->store(jxx::CAST<jxx::io::OutputStream>(out), nullptr);
    const std::string bytes = out->bytesAsString();
    EXPECT_NE(bytes.find("k=\\u03A9"), std::string::npos);
    EXPECT_TRUE(out->flushed());
}

TEST_F(PropertiesTest, ListTruncatesLongValues) {
    auto p = std::make_shared<Properties>();
    p->setProperty(S("k"), S("0123456789012345678901234567890123456789TAIL"));
    auto outStream = std::make_shared<MemoryOutputStream>();
    auto ps = std::make_shared<jxx::io::PrintStream>(jxx::CAST<jxx::io::OutputStream>(outStream), false);
    p->list(ps);
    const std::string psOut = outStream->bytesAsString();
    EXPECT_NE(psOut.find("-- listing properties --"), std::string::npos);
    EXPECT_NE(psOut.find("..."), std::string::npos);
    auto writer = std::make_shared<MemoryWriter>();
    auto pw = std::make_shared<jxx::io::PrintWriter>(jxx::CAST<jxx::io::Writer>(writer), false);
    p->list(pw);
    EXPECT_NE(writer->utf8().find("..."), std::string::npos);
}

TEST_F(PropertiesTest, SaveDelegatesToStoreOutputStream) {
    auto p = std::make_shared<Properties>();
    p->setProperty(S("key"), S("value"));
    auto out = std::make_shared<MemoryOutputStream>();
    p->save(jxx::CAST<jxx::io::OutputStream>(out), S("comment"));
    const std::string bytes = out->bytesAsString();
    EXPECT_NE(bytes.find("#comment"), std::string::npos);
    EXPECT_NE(bytes.find("key=value"), std::string::npos);
}

TEST_F(PropertiesTest, StoreToXmlAndLoadFromXmlRoundTrip) {
    auto original = std::make_shared<Properties>();
    original->setProperty(S("alpha"), S("1"));
    original->setProperty(std::make_shared<String>(u"sym"), std::make_shared<String>(u"A<&>\"'"));
    auto out = std::make_shared<MemoryOutputStream>();
    original->storeToXML(jxx::CAST<jxx::io::OutputStream>(out), S("comment"), S("UTF-8"));
    const std::string xml = out->bytesAsString();
    EXPECT_NE(xml.find("<!DOCTYPE properties SYSTEM \"http://java.sun.com/dtd/properties.dtd\">"), std::string::npos);
    EXPECT_NE(xml.find("<comment>comment</comment>"), std::string::npos);
    EXPECT_NE(xml.find("<entry key=\"alpha\">1</entry>"), std::string::npos);
    auto in = std::make_shared<MemoryInputStream>(bytesFromUtf8(xml));
    auto roundtrip = std::make_shared<Properties>();
    roundtrip->loadFromXML(jxx::CAST<jxx::io::InputStream>(in));
    ExpectStringEq(roundtrip->getProperty(S("alpha")), "1");
    auto sym = roundtrip->getProperty(S("sym"));
    ASSERT_NE(sym, nullptr);
    EXPECT_EQ(sym->utf16(), std::make_shared<String>(u"A<&>\"'")->utf16());
}

TEST_F(PropertiesTest, LoadFromXmlRejectsTrailingContent) {
    const std::string badXml =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<!DOCTYPE properties SYSTEM \"http://java.sun.com/dtd/properties.dtd\">\n"
        "<properties><entry key=\"a\">1</entry></properties>junk";
    auto in = std::make_shared<MemoryInputStream>(bytesFromUtf8(badXml));
    auto p = std::make_shared<Properties>();
    EXPECT_THROW(p->loadFromXML(jxx::CAST<jxx::io::InputStream>(in)), jxx::lang::IllegalArgumentException);
}

TEST_F(PropertiesTest, StoreToXmlRejectsUnsupportedEncoding) {
    auto p = std::make_shared<Properties>();
    p->setProperty(S("a"), S("1"));
    auto out = std::make_shared<MemoryOutputStream>();
    EXPECT_THROW(p->storeToXML(jxx::CAST<jxx::io::OutputStream>(out), S("comment"), S("UTF-16")), jxx::lang::IllegalArgumentException);
}
/*
TEST_F(PropertiesTest, NullArgumentExceptions) {
    auto p = std::make_shared<Properties>();
    EXPECT_THROW(p->setProperty(nullptr, S("v")), jxx::lang::NullPointerException);
    EXPECT_THROW((void)p->getProperty(nullptr), jxx::lang::NullPointerException);
    EXPECT_THROW(p->load(const jxx::Ptr<jxx::io::Reader>()), jxx::lang::NullPointerException);
    EXPECT_THROW(p->load(const jxx::Ptr<jxx::io::InputStream>()), jxx::lang::NullPointerException);
    EXPECT_THROW(p->store(const jxx::Ptr<jxx::io::Writer>(), S("c")), jxx::lang::NullPointerException);
    EXPECT_THROW(p->store(const jxx::Ptr<jxx::io::OutputStream>(), S("c")), jxx::lang::NullPointerException);
    EXPECT_THROW(p->list(const jxx::Ptr<jxx::io::PrintWriter>()), jxx::lang::NullPointerException);
    EXPECT_THROW(p->list(const jxx::Ptr<jxx::io::PrintStream>()), jxx::lang::NullPointerException);
    EXPECT_THROW(p->loadFromXML(const jxx::Ptr<jxx::io::InputStream>()), jxx::lang::NullPointerException);
}
*/
} // anonymous namespace
