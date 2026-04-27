#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <cstddef>
#include <algorithm>
#include <utility>
#include "../lang/jxx.lang.Object.h"

namespace jxx { namespace security {

// A password-friendly string that attempts to minimize lifetime in memory and
// wipes its buffer upon destruction/clear/move.
// Note: No absolute guarantees can be made in portable C++, but this class
// follows best-effort practices (volatile wipe, move semantics, no implicit copies).
class SecureString : public jxx::lang::Object {
public:
    SecureString() = default;

    // Construct from byte span
    SecureString(const char* data, std::size_t len) { assign(data, len); }
    explicit SecureString(const std::string& s) { assign(s.data(), s.size()); }

    // Non-copyable (to avoid accidental duplication of secrets)
    SecureString(const SecureString&) = delete;
    SecureString& operator=(const SecureString&) = delete;

    // Movable; source is securely wiped
    SecureString(SecureString&& other) noexcept { move_from(std::move(other)); }
    SecureString& operator=(SecureString&& other) noexcept {
        if (this != &other) { secure_wipe(); move_from(std::move(other)); }
        return *this;
    }

    ~SecureString() override { secure_wipe(); }

    void assign(const char* data, std::size_t len) {
        secure_wipe();
        buf_.resize(len + 1);
        std::memcpy(buf_.data(), data, len);
        buf_[len] = '\0';
    }

    void assign(const std::string& s) { assign(s.data(), s.size()); }

    void clear() { secure_wipe(); }

    void push_back(char c) {
        auto n = size();
        buf_.insert(buf_.begin() + static_cast<std::ptrdiff_t>(n), c);
        // ensure terminator
        if (buf_.empty() || buf_.back() != '\0') buf_.push_back('\0');
        else buf_[buf_.size()-1] = '\0';
    }

    std::size_t size() const noexcept {
        if (buf_.empty()) return 0; // excludes null terminator
        return buf_.size() - 1;
    }

    bool empty() const noexcept { return size() == 0; }

    const char* c_str() const noexcept { return buf_.empty() ? "" : buf_.data(); }
    char* data() noexcept { return buf_.empty() ? nullptr : buf_.data(); }
    const char* data() const noexcept { return buf_.empty() ? nullptr : buf_.data(); }

    // Secure overwrite of content, leaves object empty
    void secure_wipe() noexcept {
        if (!buf_.empty()) {
            // Best-effort: volatile pointer to prevent optimizer removal
            volatile char* p = reinterpret_cast<volatile char*>(buf_.data());
            for (std::size_t i = 0; i < buf_.size(); ++i) p[i] = 0;
            buf_.clear();
            buf_.shrink_to_fit();
        }
    }

private:
    void move_from(SecureString&& other) noexcept {
        buf_ = std::move(other.buf_);
        other.secure_wipe();
    }

    std::vector<char> buf_;
};

}} // namespace jxx::security
