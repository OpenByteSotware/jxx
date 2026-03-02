#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <optional>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <stdexcept>

#include "jxx.h"

namespace jxx::util {

    // Java 8-like Properties built atop synchronized Hashtable<String, String>.
    // - Disallows null keys/values (like Hashtable).
    // - Supports defaults chain, getProperty/setProperty, propertyNames/stringPropertyNames,
    //   load/store (.properties format), and list().
    class Properties : public Hashtable<std::string, std::string> {
    public:
        using Base = Hashtable<std::string, std::string>;
        using value_optional = std::optional<std::string>;

        // ---- ctors ----
        Properties() = default;

        // Construct with defaults (fallback lookups flow to defaults).
        explicit Properties(const Properties* defaults)
            : defaults_(defaults) {}

        // ---- Defaults handling ----
        const Properties* defaults() const noexcept { return defaults_; }
        void setDefaults(const Properties* dflt) noexcept { defaults_ = dflt; }

        // ---- Core Java-like API ----

        // getProperty(key): return value; if absent, consult defaults recursively; if still absent, return empty string.
        // getProperty(key, defaultValue): same but returns defaultValue if not found.
        std::string getProperty(const std::string& key) const {
            auto opt = getPropertyOpt(key);
            return opt.has_value() ? *opt : std::string{};
        }

        std::string getProperty(const std::string& key, const std::string& defaultValue) const {
            auto opt = getPropertyOpt(key);
            return opt.has_value() ? *opt : defaultValue;
        }

        // Optional-returning form: present if found in this or defaults chain.
        std::optional<std::string> getPropertyOpt(const std::string& key) const {
            // Hashtable::get is synchronized and disallows nulls.
            auto val = Base::get(key);
            if (val.has_value()) return val;
            // Recurse into defaults (its getPropertyOpt is also synchronized).
            if (defaults_) return defaults_->getPropertyOpt(key);
            return std::nullopt;
        }

        // setProperty(key, value): returns previous value (or nullopt if absent).
        // (Enforces non-null as Hashtable does.)
        std::optional<std::string> setProperty(const std::string& key, const std::string& value) {
            return Base::put(key, value_optional{ value });
        }

        // propertyNames(): like Java — returns keys from this + defaults (unique, no ordering guarantee).
        std::vector<std::string> propertyNames() const {
            std::unordered_set<std::string> set;
            // Gather this properties' keys (synchronized in Base::keySet)
            for (const auto& k : Base::keySet()) set.insert(k);
            // And from defaults recursively
            if (defaults_) {
                for (const auto& k : defaults_->propertyNames()) set.insert(k);
            }
            return std::vector<std::string>(set.begin(), set.end());
        }

        // stringPropertyNames(): same as propertyNames() for our String-only implementation.
        // Java returns Set<String>; we return unordered_set for uniqueness, plus a vector helper.
        std::unordered_set<std::string> stringPropertyNames() const {
            std::unordered_set<std::string> set;
            for (const auto& k : Base::keySet()) set.insert(k);
            if (defaults_) {
                auto d = defaults_->stringPropertyNames();
                set.insert(d.begin(), d.end());
            }
            return set;
        }
        std::vector<std::string> stringPropertyNamesVector() const {
            auto s = stringPropertyNames();
            return std::vector<std::string>(s.begin(), s.end());
        }

        // ---- I/O (.properties format) ----
        // Java-like load/store with escaping, continuations, comments.
        // Notes:
        //  - We write a header with a comment line and a date line (like Java's store()).
        //  - We escape special characters and non-ASCII as \uXXXX.
        //  - store() writes UTF-8 bytes by default; escapes keep content portable.
        //  - load() supports # / ! comments, \ continuations, = : and whitespace separators.
        void load(std::istream& in) {
            std::string raw, logical;
            while (std::getline(in, raw)) {
                // Handle line continuations: lines ending with an odd number of trailing backslashes.
                if (!logical.empty()) {
                    logical.pop_back(); // remove the continuation backslash from previous chunk
                    logical += trimLeftPreserve(raw);
                }
                else {
                    // Start of a new logical line
                    if (isCommentOrBlank(raw)) continue;
                    logical = raw;
                    // If it's just a comment/blank we handled above
                    if (logical.empty()) continue;
                }

                // If this line continues, keep accumulating
                if (endsWithContinuation(logical)) {
                    continue; // accumulate more
                }

                // Now parse the full logical line
                parseAndPut(logical);
                logical.clear();
            }

            // If there was a dangling logical line without terminating continuation, parse it
            if (!logical.empty()) {
                parseAndPut(logical);
            }
        }

        void store(std::ostream& out, const std::string& comments = {}) const {
            // Comment header
            if (!comments.empty()) {
                writeCommentLine(out, comments);
            }
            writeDateLine(out);

            // We don't guarantee ordering (Hashtable is unordered).
            for (const auto& e : Base::entrySet()) {
                const auto& k = e.getKey();
                const auto& vOpt = e.getValue();
                if (!vOpt.has_value()) continue; // Hashtable never stores nulls, but guard anyway
                auto kEsc = escapeKey(k);
                auto vEsc = escapeValue(*vOpt);
                out << kEsc << '=' << vEsc << '\n';
            }
        }

        // Human-readable dump (no header)
        void list(std::ostream& out) const {
            for (const auto& e : Base::entrySet()) {
                const auto& k = e.getKey();
                const auto& vOpt = e.getValue();
                if (!vOpt.has_value()) continue;
                out << k << '=' << *vOpt << '\n';
            }
            if (defaults_) {
                out << "-- Defaults --\n";
                defaults_->list(out);
            }
        }

        // Convenience overloads matching Java-like names
        std::optional<std::string> put(const std::string& k, const std::string& v) {
            return Base::put(k, value_optional{ v });
        }

    private:
        const Properties* defaults_ = nullptr; // not owned; mirrors Java's reference

        // ---------- Parsing helpers ----------
        static bool isWhitespace(char c) {
            return c == ' ' || c == '\t' || c == '\f';
        }

        static std::string trimLeftPreserve(const std::string& s) {
            size_t i = 0;
            while (i < s.size() && isWhitespace(s[i])) ++i;
            return s.substr(i);
        }

        static bool isCommentOrBlank(const std::string& s) {
            // Skip leading whitespace
            size_t i = 0;
            while (i < s.size() && isWhitespace(s[i])) ++i;
            if (i >= s.size()) return true; // blank
            return s[i] == '#' || s[i] == '!';
        }

        static bool endsWithContinuation(const std::string& s) {
            // Count trailing backslashes
            size_t n = 0, i = s.size();
            while (i > 0 && s[i - 1] == '\\') { --i; ++n; }
            return (n % 2) == 1; // odd => continuation
        }

        void parseAndPut(const std::string& logical) {
            // Skip comments/blank again (safe if called directly)
            if (isCommentOrBlank(logical)) return;

            // Find unescaped separator ('=', ':', or whitespace)
            size_t sep = std::string::npos;
            bool escaped = false;
            for (size_t i = 0; i < logical.size(); ++i) {
                char c = logical[i];
                if (!escaped && (c == '=' || c == ':' || isWhitespace(c))) {
                    sep = i;
                    break;
                }
                escaped = (!escaped && c == '\\');
                if (escaped && c != '\\') escaped = false; // only track escapes for immediate next
            }

            std::string kPart, vPart;
            if (sep == std::string::npos) {
                kPart = logical;
                vPart.clear();
            }
            else {
                kPart = logical.substr(0, sep);
                // Determine value start
                size_t j = sep;
                // If actual separator is whitespace, skip any number of whitespace and a single '=' or ':' if present
                if (isWhitespace(logical[j])) {
                    while (j < logical.size() && isWhitespace(logical[j])) ++j;
                    if (j < logical.size() && (logical[j] == '=' || logical[j] == ':')) ++j;
                }
                else {
                    // it was '=' or ':'; advance past it
                    ++j;
                }
                // Skip leading whitespace in value
                while (j < logical.size() && isWhitespace(logical[j])) ++j;
                vPart = logical.substr(j);
            }

            auto key = unescape(kPart);
            auto val = unescape(vPart);
            (void)Base::put(key, value_optional{ val });
        }

        // ---------- Escape/Unescape ----------
        static std::string unescape(const std::string& s) {
            std::string out;
            out.reserve(s.size());
            for (size_t i = 0; i < s.size(); ++i) {
                char c = s[i];
                if (c != '\\') { out.push_back(c); continue; }
                if (++i >= s.size()) { out.push_back('\\'); break; }
                char e = s[i];
                switch (e) {
                case 't': out.push_back('\t'); break;
                case 'n': out.push_back('\n'); break;
                case 'r': out.push_back('\r'); break;
                case 'f': out.push_back('\f'); break;
                case '\\': out.push_back('\\'); break;
                case ' ': out.push_back(' '); break;
                case ':': out.push_back(':'); break;
                case '=': out.push_back('='); break;
                case '#': out.push_back('#'); break;
                case '!': out.push_back('!'); break;
                case 'u': {
                    // \uXXXX
                    if (i + 4 >= s.size()) { out.push_back('u'); break; }
                    unsigned code = 0;
                    bool ok = true;
                    for (int k = 0; k < 4; ++k) {
                        char h = s[i + 1 + k];
                        code <<= 4;
                        if (h >= '0' && h <= '9') code += (h - '0');
                        else if (h >= 'a' && h <= 'f') code += (h - 'a' + 10);
                        else if (h >= 'A' && h <= 'F') code += (h - 'A' + 10);
                        else { ok = false; break; }
                    }
                    if (ok) {
                        appendUtf8(out, code);
                        i += 4;
                    }
                    else {
                        out.push_back('u'); // leave as-is on malformed
                    }
                    break;
                }
                default:
                    // Unknown escape — preserve char
                    out.push_back(e);
                    break;
                }
            }
            return out;
        }

        static std::string escapeKey(const std::string& s) {
            std::string out; out.reserve(s.size() * 2);
            bool leading = true;
            for (char c : s) {
                switch (c) {
                case ' ': out += (leading ? "\\ " : " "); break;
                case '\t': out += "\\t"; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\f': out += "\\f"; break;
                case '\\': out += "\\\\"; break;
                case '=': case ':': case '#': case '!':
                    out.push_back('\\'); out.push_back(c); break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20 || static_cast<unsigned char>(c) > 0x7E) {
                        appendUnicodeEscape(out, static_cast<unsigned char>(c));
                    }
                    else {
                        out.push_back(c);
                    }
                    break;
                }
                leading = false;
            }
            return out;
        }

        static std::string escapeValue(const std::string& s) {
            std::string out; out.reserve(s.size() * 2);
            bool leading = true;
            for (char c : s) {
                switch (c) {
                case ' ': out += (leading ? "\\ " : " "); break;
                case '\t': out += "\\t"; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\f': out += "\\f"; break;
                case '\\': out += "\\\\"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20 || static_cast<unsigned char>(c) > 0x7E) {
                        appendUnicodeEscape(out, static_cast<unsigned char>(c));
                    }
                    else {
                        out.push_back(c);
                    }
                    break;
                }
                leading = false;
            }
            return out;
        }

        static void appendUnicodeEscape(std::string& out, unsigned codeUnit) {
            std::ostringstream oss;
            oss << "\\u" << std::hex << std::uppercase << std::setfill('0') << std::setw(4)
                << (codeUnit & 0xFFFFu);
            out += oss.str();
        }

        static void appendUtf8(std::string& out, unsigned codePoint) {
            // Encode codePoint as UTF-8
            if (codePoint <= 0x7F) {
                out.push_back(static_cast<char>(codePoint));
            }
            else if (codePoint <= 0x7FF) {
                out.push_back(static_cast<char>(0xC0 | (codePoint >> 6)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            }
            else if (codePoint <= 0xFFFF) {
                out.push_back(static_cast<char>(0xE0 | (codePoint >> 12)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            }
            else {
                out.push_back(static_cast<char>(0xF0 | (codePoint >> 18)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            }
        }

        static void writeCommentLine(std::ostream& out, const std::string& comments) {
            std::istringstream iss(comments);
            std::string line;
            while (std::getline(iss, line)) {
                out << '#' << line << '\n';
            }
        }

        static void writeDateLine(std::ostream& out) {
            // Java uses Date.toString(); we’ll emit a similar human-readable line.
            std::time_t t = std::time(nullptr);
            char buf[128]{};
            if (std::strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Z %Y", std::localtime(&t))) {
                out << '#' << buf << '\n';
            }
        }
    };

} // namespace