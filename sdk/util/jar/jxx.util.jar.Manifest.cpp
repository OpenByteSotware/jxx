#include "util/jar/jxx.util.jar.Manifest.h"

#include <sstream>
#include <string>
#include <vector>

#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jar/jxx.util.jar.JarException.h"

namespace jxx::util::jar {
namespace {
std::string readAll(const ::jxx::Ptr<::jxx::io::InputStream>& input) {
    std::string result;
    for (;;) {
        const auto value = input->read();
        if (value < 0) break;
        result.push_back(static_cast<char>(value & 0xff));
    }
    return result;
}
void writeText(const ::jxx::Ptr<::jxx::io::OutputStream>& output,
    const std::string& text) {
    for (const unsigned char value : text) output->write(value);
}
std::vector<std::string> logicalLines(const std::string& text) {
    std::vector<std::string> result;
    std::istringstream input(text);
    std::string physical;
    while (std::getline(input, physical)) {
        if (!physical.empty() && physical.back() == '\r') physical.pop_back();
        if (!physical.empty() && physical.front() == ' ' && !result.empty()) {
            result.back().append(physical.substr(1));
        } else {
            result.push_back(physical);
        }
    }
    return result;
}
void emitHeader(const ::jxx::Ptr<::jxx::io::OutputStream>& output,
    const std::string& name, const std::string& value) {
    std::string line = name + ": " + value;
    while (line.size() > 72U) {
        writeText(output, line.substr(0, 72U) + "\r\n");
        line = " " + line.substr(72U);
    }
    writeText(output, line + "\r\n");
}
}

Manifest::Manifest()
    : main_(::jxx::NEW<Attributes>()),
      entries_(::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, Attributes>>()) {}

Manifest::Manifest(const ::jxx::Ptr<Manifest>& other) : Manifest() {
    if (other == nullptr) return;
    main_ = ::jxx::NEW<Attributes>(other->main_);
    const auto names = other->entries_->keySet()->iterator();
    while (names->hasNext()) {
        const auto name = names->next();
        entries_->put(name, ::jxx::NEW<Attributes>(
            other->entries_->get(::jxx::CAST<::jxx::lang::Object>(name))));
    }
}

Manifest::Manifest(const ::jxx::Ptr<::jxx::io::InputStream>& input) : Manifest() {
    read(input);
}

::jxx::Ptr<Attributes> Manifest::getMainAttributes() const { return main_; }
::jxx::Ptr<::jxx::util::Map<::jxx::lang::String, Attributes>> Manifest::getEntries() const {
    return ::jxx::CAST<::jxx::util::Map<::jxx::lang::String, Attributes>>(entries_);
}
::jxx::Ptr<Attributes> Manifest::getAttributes(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    return entries_->get(::jxx::CAST<::jxx::lang::Object>(name));
}
void Manifest::clear() { main_->clear(); entries_->clear(); }

void Manifest::write(const ::jxx::Ptr<::jxx::io::OutputStream>& output) const {
    if (output == nullptr) throw ::jxx::lang::NullPointerException();
    auto version = main_->getValue(Attributes::MANIFEST_VERSION);
    if (version == nullptr) version = main_->getValue(Attributes::SIGNATURE_VERSION);
    if (version != nullptr) emitHeader(output, "Manifest-Version", version->utf8());
    for (const auto& item : main_->values_) {
        if (item.first == "manifest-version" || item.first == "signature-version") continue;
        emitHeader(output, item.first, item.second == nullptr ? "" : item.second->utf8());
    }
    writeText(output, "\r\n");
    auto iterator = entries_->keySet()->iterator();
    while (iterator->hasNext()) {
        const auto name = iterator->next();
        emitHeader(output, "Name", name->utf8());
        const auto attrs = entries_->get(::jxx::CAST<::jxx::lang::Object>(name));
        if (attrs != nullptr) {
            for (const auto& item : attrs->values_) {
                emitHeader(output, item.first,
                    item.second == nullptr ? "" : item.second->utf8());
            }
        }
        writeText(output, "\r\n");
    }
}

void Manifest::read(const ::jxx::Ptr<::jxx::io::InputStream>& input) {
    if (input == nullptr) throw ::jxx::lang::NullPointerException();
    clear();
    auto lines = logicalLines(readAll(input));
    auto current = main_;
    for (const auto& line : lines) {
        if (line.empty()) { current = nullptr; continue; }
        const auto colon = line.find(':');
        if (colon == std::string::npos || colon + 1U >= line.size() || line[colon + 1U] != ' ') {
            throw JarException("invalid manifest header");
        }
        const auto name = line.substr(0, colon);
        const auto value = line.substr(colon + 2U);
        if (current == nullptr) {
            if (name != "Name") throw JarException("manifest section lacks Name");
            const auto sectionName = ::jxx::NEW<::jxx::lang::String>(value);
            current = entries_->get(::jxx::CAST<::jxx::lang::Object>(sectionName));
            if (current == nullptr) {
                current = ::jxx::NEW<Attributes>();
                entries_->put(sectionName, current);
            }
        } else {
            current->putValue(::jxx::NEW<::jxx::lang::String>(name),
                ::jxx::NEW<::jxx::lang::String>(value));
        }
    }
}

::jxx::lang::jbool Manifest::equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const {
    const auto other = ::jxx::CAST<Manifest>(object);
    return other != nullptr && main_->equals(other->main_) && entries_->equals(other->entries_);
}
::jxx::lang::jint Manifest::hashCode() const {
    return main_->hashCode() + entries_->hashCode();
}
} // namespace jxx::util::jar
